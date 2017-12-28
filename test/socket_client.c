/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    socket_client.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:40
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:40
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#include "socket_helper.h"
#include "hex_helper.h"
#include "pthread_helper.h"
#include "network_protocol.h"
#include "network_protocol_client.h"

static socket_t *client_sk;
static int cur_status;

void handle_server_message_cb(unsigned char *message, int len)
{
	char cmd_type = message[4];

	switch (cmd_type) {
		case 1:
			printf("recv ---1\n");
			cur_status++;
			break;
		case 3:
			printf("recv ---3\n");
			cur_status++;
			break;
		default:
			break;
	}

	print_hex(message, len);
}

void client_send_config_info(void)
{
	unsigned char cmd_send_config_info[] = {0xaa, 0x0a, 0x0, 0x1, 0x2, 0x01, 0x02, 0x03, 0x04, 0xbb, 0x55};
	printf("send ------2\n");

	cur_status++;
	client_send_message(client_sk, cmd_send_config_info, sizeof(cmd_send_config_info));
}

void client_send_num_file_name(void)
{
	unsigned char cmd_send_num_file_name[] = {0xaa, 0x0c, 0x0, 0x1, 0x4, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xbb, 0x55};
	printf("send ------4\n");

	cur_status++;
	client_send_message(client_sk, cmd_send_num_file_name, sizeof(cmd_send_num_file_name));
}

void client_send_play(void)
{
	unsigned char cmd_send_play[] = {0xaa, 0x06, 0x0, 0x1, 0x5, 0xbb, 0x55};
	printf("send ------5\n");

	cur_status++;
	client_send_message(client_sk, cmd_send_play, sizeof(cmd_send_play));
}

void client_send_next(void)
{
	unsigned char cmd_send_next[] = {0xaa, 0x06, 0x0, 0x1, 0x6, 0xbb, 0x55};
	printf("send ------6\n");

	cur_status++;
	client_send_message(client_sk, cmd_send_next, sizeof(cmd_send_next));
}

void *client_send_message_thread(void *args)
{
	int sleep_cnt = 0;

	while (!get_client_read_running_flag())
		usleep(100);

	while (get_client_read_running_flag()) {
		usleep(1 * 1000 * 1000);
		printf("cur_status: %d \n", cur_status);

		switch (cur_status) {
			case 1:
				client_send_config_info();
				break;
			case 3:
				client_send_num_file_name();
				break;
			case 4:
				client_send_play();
				break;
			case 5:
				if (sleep_cnt++ > 5) {
					printf("------play music\n");
					usleep(1 * 1000 * 1000);
					sleep_cnt = 0;
					client_send_next();
				}
				break;
			case 6:
				cur_status = 4;
				break;
			default:
				break;
		}
	}
	
	return NULL;
}

int socket_client(void)
{
	int client_read_timeout_ms = 1000;

	create_a_attached_thread(NULL, client_send_message_thread, NULL);

	client_sk = network_protocol_client_init(handle_server_message_cb, client_read_timeout_ms);

	while (1) {
		usleep(1 * 1000 * 1000);
	}

	socket_clean_client(client_sk);

    return 0;
}
