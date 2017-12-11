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

static void handle_recv_message(unsigned char *buf, int len)
{
	char cmd_type;
	cmd_type = buf[4];

	print_hex(buf, len);

	switch (cmd_type) {
		case 1:
			printf("recv ---1\n");
			break;
		case 2:
			printf("recv ---2\n");
			break;
		case 3:
			printf("recv ---3\n");
			break;
		default:
			break;
	}
}

static void handle_send_message(int cmd_type)
{
	unsigned char buf[] = { \
		0xaa, 0x07, 0x0, 0x1, 0x2, 0x7, 0xbb, 0x55, \
	};

	switch (cmd_type) {
		case 1:
			send_message(buf, sizeof(buf));
			break;
		case 2:
			break;
		default:
			break;
	}
}

void *handle_send_message_thread(void *args)
{
	sleep(5);

	while (!get_client_running_flag()) {
		sleep(1);
		handle_send_message(1);
	}
	
	return NULL;
}

int socket_server(void)
{
	int read_timeout_ms = 3000;

	thread_create_detached(handle_send_message_thread, NULL);

	network_protocol_server_init(handle_recv_message, read_timeout_ms);

    return 0;
}

