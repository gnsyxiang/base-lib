/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    network_protocol_server.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    16/12 2017 23:59
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        16/12 2017      create the file
 * 
 *     last modified: 16/12 2017 23:59
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
#include "pthread_helper.h"
#include "network_protocol.h"

#define NETWORK_PROTOCOL_SERVER_GB
#include "network_protocol_server.h"
#undef NETWORK_PROTOCOL_SERVER_GB

#define READ_MESSAGE_LEN (7)

static unsigned char *send_buf_l;
static int send_buf_len_l;

static int is_server_read_running;

int get_server_read_running_flag(void)
{
	return is_server_read_running;
}

void server_send_message(socket_t *sk, unsigned char *buf, int len)
{
	send_buf_l = (unsigned char *)malloc(len + 1);
	memset(send_buf_l, '\0', len + 1);

	memcpy(send_buf_l, buf, len);
	send_buf_len_l = len;

	send_ok(sk);
}

static void *server_send_message_thread(void *args)
{
	socket_t *client_sk = (socket_t *)args;

	while (is_server_read_running) {
		printf("--------------1\n");
		send_wait(client_sk);
		printf("--------------2\n");

		print_hex(send_buf_l, send_buf_len_l);
		socket_write(client_sk, (char *)send_buf_l, send_buf_len_l);

		free(send_buf_l);
	}

	return NULL;
}

static void *server_read_thread(void *args)
{    
	int ret;
	socket_t *client_sk = (socket_t *)args;

	socket_set_recv_timeout(client_sk, client_sk->read_timeout_ms);
	is_server_read_running = 1;

	create_a_attached_thread(NULL, server_send_message_thread, args);

	while(is_server_read_running) {
		unsigned char buf[BUF_LEN] = {0};

		ret = socket_read(client_sk, (char *)buf, READ_MESSAGE_LEN);
		if (ret == 0) {
			printf("client socket close \n");
			is_server_read_running = 0;
			usleep(1 * 1000);
			break;
		}

		if (ret > 0)
			check_is_package(buf, ret, client_sk->handle_read_message);
	}

	return NULL;
}

socket_t *network_protocol_server_init(handle_message_t handle_read_message, int read_timeout_ms)
{
	assert(handle_read_message);

	socket_t *sk_server = socket_init_server(MYPORT, handle_read_message, read_timeout_ms);
	socket_t *client_sk = socket_wait_for_connect(sk_server, server_read_thread);

	socket_clean_client(sk_server);

	return client_sk;
}

