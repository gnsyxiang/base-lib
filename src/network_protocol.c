/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    network_protocol.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/12 2017 22:46
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/12 2017      create the file
 * 
 *     last modified: 08/12 2017 22:46
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <assert.h>
#include <stdlib.h>

#include "socket_helper.h"
#include "hex_helper.h"
#include "thread_helper.h"

#define NETWORK_PROTOCOL_GB
#include "network_protocol.h"
#undef NETWORK_PROTOCOL_GB

#define READ_MESSAGE_LEN (7)

static unsigned char *send_buf_l;
static int send_buf_len_l;

static int is_client_running;
static int is_client_read_running;
static pthread_mutex_t send_mutex;
static pthread_cond_t  send_cond;

static void check_is_package(unsigned char *buf, int ret, handle_message_t handle_message)
{
	int cur_status = 1;
	int package_len;
	unsigned char *pbuf;
	static unsigned char buf_tmp[BUF_LEN] = {0};
	static int store_left_bytes = 0;

	memset(buf_tmp + store_left_bytes, '\0', BUF_LEN - store_left_bytes);
	memcpy(buf_tmp + store_left_bytes, buf, ret);
	pbuf = buf_tmp;

	ret += store_left_bytes;
	store_left_bytes = 0;

	while ((ret) > 0) {
		switch (cur_status) {
			case 1:
				while (ret && *pbuf != FRONT_SYMBOL) {
					pbuf++;
					ret--;
				}
				cur_status++;
				break;
			case 2:
				package_len = pbuf[1] | pbuf[2] << 8;

				if (package_len && pbuf[package_len] != TAIL_SYMBOL && pbuf[package_len] != 0x0) {
					pbuf++;
					ret--;
					cur_status = 1;
					break;
				}
				cur_status++;
				break;
			case 3:
				if (pbuf[package_len] == TAIL_SYMBOL) {
					handle_message(pbuf, package_len + 1);

					pbuf += package_len + 1;
					ret -= package_len + 1;

					cur_status = 1;
					break;
				}
				cur_status++;
				break;
			case 4:
				memcpy(buf_tmp, pbuf, ret);

				store_left_bytes = ret;
				ret = 0;
				break;
			default:
				printf("error status \n");
				break;
		}
	}
}

static void send_wait(void)
{
	pthread_mutex_lock(&send_mutex);
	pthread_cond_wait(&send_cond, &send_mutex);
	pthread_mutex_unlock(&send_mutex);
}

static void send_ok(void)
{
	pthread_mutex_lock(&send_mutex);
	pthread_cond_signal(&send_cond);
	pthread_mutex_unlock(&send_mutex);
}

void send_message(unsigned char *buf, int len)
{
	send_buf_l = (unsigned char *)malloc(len + 1);
	memset(send_buf_l, '\0', len + 1);

	memcpy(send_buf_l, buf, len);
	send_buf_len_l = len;

	send_ok();
}
static void send_ok_haha(socket_t *sk)
{
	pthread_mutex_lock(&sk->mutex);
	pthread_cond_signal(&sk->cond);
	pthread_mutex_unlock(&sk->mutex);
}
static void send_wait_haha(socket_t *sk)
{
	pthread_mutex_lock(&sk->mutex);
	pthread_cond_wait(&sk->cond, &sk->mutex);
	pthread_mutex_unlock(&sk->mutex);
}
void send_message_haha(socket_t *sk, unsigned char *buf, int len)
{
	send_buf_l = (unsigned char *)malloc(len + 1);
	memset(send_buf_l, '\0', len + 1);

	memcpy(send_buf_l, buf, len);
	send_buf_len_l = len;

	send_ok_haha(sk);
}

int get_client_read_running_flag(void)
{
	return is_client_read_running;
}

static void *client_send_message_thread(void *args)
{
	socket_t *client_sk = (socket_t *)args;

	while (is_client_read_running) {
		send_wait_haha(client_sk);

		print_hex(send_buf_l, send_buf_len_l);
		socket_write(client_sk, (char *)send_buf_l, send_buf_len_l);

		free(send_buf_l);
	}

	return NULL;
}

int get_client_running_flag(void)
{
	return is_client_running;
}

static void *send_message_thread(void *args)
{
	socket_t *client_sk = (socket_t *)args;

	pthread_mutex_init(&send_mutex, NULL);
	pthread_cond_init(&send_cond, NULL);

	while (is_client_running) {
		send_wait();

		print_hex(send_buf_l, send_buf_len_l);
		socket_write(client_sk, (char *)send_buf_l, send_buf_len_l);

		free(send_buf_l);
	}

	return NULL;
}

static void *client_thread_callback(void *args)
{    
	int ret;
	socket_t *client_sk = (socket_t *)args;

	socket_set_recv_timeout(client_sk, client_sk->read_timeout_ms);
	is_client_running = 1;

	thread_create_detached(send_message_thread, args);

	while(is_client_running) {
		unsigned char buf[BUF_LEN] = {0};

		ret = socket_read(client_sk, (char *)buf, READ_MESSAGE_LEN);
		if (ret == 0) {
			printf("client socket close \n");
			is_client_running = 0;
			usleep(1 * 1000);
			break;
		}

		if (ret > 0)
			check_is_package(buf, ret, client_sk->handle_read_message);
	}

	socket_clean_client(client_sk);

	return NULL;
}

static void *client_read_thread(void *args)
{    
	int ret;
	socket_t *client_sk = (socket_t *)args;

	socket_set_recv_timeout(client_sk, client_sk->read_timeout_ms);
	is_client_read_running = 1;

	thread_create_detached(client_send_message_thread, args);

	while(is_client_read_running) {
		unsigned char buf[BUF_LEN] = {0};

		ret = socket_read(client_sk, (char *)buf, READ_MESSAGE_LEN);
		if (ret == 0) {
			printf("client socket close \n");
			is_client_read_running = 0;
			usleep(1 * 1000);
			break;
		}

		if (ret > 0)
			check_is_package(buf, ret, client_sk->handle_read_message);
	}

	return NULL;
}

void network_protocol_server_init(handle_message_t handle_read_message, int read_timeout_ms)
{
	assert(handle_read_message);

	socket_t *sk_server = socket_init_server(MYPORT, handle_read_message, read_timeout_ms);
	socket_wait_for_connect(sk_server, client_thread_callback);
    close(sk_server->fd);
}

socket_t *network_protocol_client_init(handle_message_t handle_read_message, int read_timeout_ms)
{
	assert(handle_read_message);
	
	socket_t *sk_client = socket_init_client("127.0.0.1", MYPORT, handle_read_message, read_timeout_ms);
	socket_connect(sk_client, client_read_thread, 3);

	return sk_client;
}

