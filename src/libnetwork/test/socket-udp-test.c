/**
 * 
 * Release under GPLv2.
 * 
 * @file    socket-udp-test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    11/07 2018 16:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        11/07 2018      create the file
 * 
 *     last modified: 11/07 2018 16:48
 */
#include <stdio.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>

#include <log_helper.h>
#include <parse_cmd.h>

#include "socket.h"

const int MSG_LEN = 1024;

void socket_udp_client_cb(void *argv)
{
	socket_t *sk = (socket_t *)argv;
	log_i("fd: %d ", sk->fd);

	int ret;
	char send_msg[MSG_LEN];
	char recv_msg[MSG_LEN+1];

	socket_udp_set_sockaddr_in(sk, sk->port, sk->ip);

	memset(send_msg, '\0', sizeof(send_msg));
	while (fgets(send_msg, MSG_LEN, stdin) != NULL) {
		ret = socket_udp_send_msg(sk, send_msg, strlen(send_msg));

		memset(recv_msg, '\0', sizeof(recv_msg));
		ret = socket_udp_recv_msg(sk, recv_msg, MSG_LEN);
		log_i("ret: %d, msg: %s", ret, recv_msg);

		memset(send_msg, '\0', sizeof(send_msg));
	}
}

void socket_udp_client_test(void)
{
	socket_t *sk = socket_udp_client_init(TEST_PORT, TEST_IP);

	socket_connect(sk, socket_udp_client_cb, 3, NULL);

	socket_udp_client_clean(sk);
}

void socket_udp_server_cb(void *argv)
{
	socket_t *sk = (socket_t *)argv;
	log_i("fd: %d ", sk->fd);

	int ret;
	char msg[MSG_LEN];

	while (1) {
		memset(msg, '\0', sizeof(msg));

		ret = socket_udp_recv_msg(sk, msg, MSG_LEN);
		log_i("ret: %d, msg: %s", ret, msg);

		socket_udp_send_msg(sk, msg, ret);
	}
}

void socket_udp_server_test(void)
{
	socket_t *sk = socket_udp_server_init(TEST_PORT, TEST_IP);

	socket_udp_wait_for_connect(sk, socket_udp_server_cb);

	socket_udp_client_clean(sk);
}

static void socket_udp_test_init(void)
{
	printf("4. socket udp client test \n");
	printf("5. socket udp server test \n");

	handle_test_cmd_t socket_udp_test_cmd[] = {
		{"4", socket_udp_client_test},
		{"5", socket_udp_server_test},
	};

	register_test_cmd(socket_udp_test_cmd, ARRAY_NUM(socket_udp_test_cmd));
}
DECLARE_INIT(socket_udp_test_init);

