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
#include "pthread_helper.h"
#include "network_protocol.h"
#include "network_protocol_server.h"
#include "parse_cmd.h"

static socket_t *client_sk;

static void handle_recv_message(unsigned char *message, int len)
{
	print_hex(message, len);
	printf("cmd_type: %d \n\n", message[4]);

}

static void *handle_send_message_thread(void *args)
{
	unsigned char buf[] = {0xaa, 0x06, 0x00, 0x01, 0x02, 0xbb, 0x55};

	while (!get_server_read_running_flag())
		usleep(100);

	while (get_server_read_running_flag()) {
		usleep(1 * 1000 * 1000);

		server_send_message(client_sk, buf, sizeof(buf));
	}
	
	return NULL;
}

static int socket_server(void)
{
	int read_timeout_ms = 1000;

	create_a_attached_thread(NULL, handle_send_message_thread, NULL);

	client_sk = network_protocol_server_init(handle_recv_message, read_timeout_ms);

	while (1) {
		usleep(1 * 1000 * 1000);
	}

	socket_clean_client(client_sk);

    return 0;
}

void socket_server_test_init(void)
{
	handle_test_cmd_t socket_server_test_cmd[] = {
		{"2", socket_server},
	};

	register_test_cmd(socket_server_test_cmd, ARRAY_NUM(socket_server_test_cmd));
}

