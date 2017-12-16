/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    socket_server.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    11/12 2017 20:42
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        11/12 2017      create the file
 * 
 *     last modified: 11/12 2017 20:42
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hex_helper.h"
#include "network_protocol.h"
#include "thread_helper.h"

static socket_t *client_sk;
static int cur_status;

static void handle_recv_message(unsigned char *buf, int len)
{
	char cmd_type;
	cmd_type = buf[4];

	switch (cmd_type) {
		case 2:
			printf("recv ---2\n");
			cur_status++;
			break;
		case 4:
			printf("recv ---4\n");
			cur_status++;
			break;
		case 5:
			printf("recv ---5\n");
			break;
		case 6:
			printf("recv ---6\n");
			break;
		default:
			break;
	}

	print_hex(buf, len);
}

void handle_send_get_config_info(void)
{
	unsigned char cmd_get_config_info[] = {0xaa, 0x06, 0x0, 0x1, 0x1, 0xbb, 0x55};
	printf("send ------1\n");

	cur_status++;
	send_message(client_sk, cmd_get_config_info, sizeof(cmd_get_config_info));
}

void handle_send_ready(void)
{
	unsigned char cmd_ready[] = {0xaa, 0x06, 0x0, 0x1, 0x3, 0xbb, 0x55};
	printf("send ------3\n");

	cur_status++;
	send_message(client_sk, cmd_ready, sizeof(cmd_ready));
}

void *handle_send_message_thread(void *args)
{
	while (!get_server_read_running_flag())
		usleep(100);

	while (get_server_read_running_flag()) {
		switch (cur_status) {
			case 0:
				handle_send_get_config_info();
				break;
			case 2:
				handle_send_ready();
				break;
			default:
				usleep(1 * 1000 * 1000);
				printf("cur_status: %d \n", cur_status);
				break;
		}
	}
	
	return NULL;
}

int socket_server(void)
{
	int read_timeout_ms = 1000;

	thread_create_detached(handle_send_message_thread, NULL);

	client_sk = network_protocol_server_init(handle_recv_message, read_timeout_ms);

	while (1) {
		usleep(1 * 1000 * 1000);
	}

	socket_clean_client(client_sk);

    return 0;
}

