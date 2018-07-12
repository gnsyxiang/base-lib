/**
 * 
 * Release under GPLv2.
 * 
 * @file    socket-tcp-test.c
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

#include <log_helper.h>
#include <parse_cmd.h>

#include "socket.h"

void socket_tcp_client_cb(void *argv)
{
	socket_t *sk = (socket_t *)argv;

	log_i("fd: %d ", sk->fd);
}

void socket_tcp_client_test(void)
{
	socket_t *sk = socket_tcp_client_init(TEST_PORT, TEST_IP);

	socket_connect(sk, socket_tcp_client_cb, 3);

	socket_tcp_client_clean(sk);
}

void socket_tcp_server_cb(void *argv)
{
	socket_t *sk_client = (socket_t *)argv;

	log_i("fd: %d ", sk_client->fd);

	socket_tcp_client_clean(sk_client);
}

void socket_tcp_server_test(void)
{
	socket_t *sk = socket_tcp_server_init(TEST_PORT, TEST_IP);

	socket_tcp_wait_for_connect(sk, socket_tcp_server_cb);

	socket_tcp_client_clean(sk);
}

static void socket_test_init(void)
{
	printf("2. socket tcp client test \n");
	printf("3. socket tcp server test \n");

	handle_test_cmd_t socket_test_cmd[] = {
		{"2", socket_tcp_client_test},
		{"3", socket_tcp_server_test},
	};

	register_test_cmd(socket_test_cmd, ARRAY_NUM(socket_test_cmd));
}
DECLARE_INIT(socket_test_init);

