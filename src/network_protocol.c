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

#include "socket_helper.h"
#include "hex_helper.h"

#define NETWORK_PROTOCOL_GB
#include "network_protocol.h"
#undef NETWORK_PROTOCOL_GB

static handle_message_t handle_message_callback;

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

static void *client_thread_callback(void *args)
{    
	int i = 0;
	int ret;
	socket_t *client_sk = (socket_t *)args;

	while(1) {
		unsigned char buf[BUF_LEN] = {0};

		ret = socket_read(client_sk, (char *)buf, ++i);
		if (ret <= 0) {
			printf("client socket close \n");
			break;
		}

		check_is_package(buf, ret, handle_message_callback);
	}

	socket_clean_client(client_sk);

	return NULL;
}

static int init_server(void)
{
	socket_t *sk_server = socket_init_server(MYPORT);
	socket_wait_for_connect(sk_server, client_thread_callback);
    close(sk_server->fd);

    return 0;
}

void network_protocol_init(handle_message_t handle_message)
{
	assert(handle_message);

	handle_message_callback = handle_message;

	init_server();
}

