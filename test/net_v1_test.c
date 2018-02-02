/**
 * 
 * Release under GPLv2.
 * 
 * @file    net_v1_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/02 2018 12:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/02 2018      create the file
 * 
 *     last modified: 02/02 2018 12:32
 */
#include <stdio.h>

#include "net_v1.h"
#include "parse_cmd.h"
#include "log_helper.h"

int cnt;

static void socket_client_cb(void *args)
{
	socket_t *sk_accept = args;

	log_i("fd: %d", sk_accept->fd);
}

static void net_v1_client_test(void)
{
	net_v1_client_init(socket_client_cb);
}

static void socket_server_cb(void *args)
{
	if (cnt++ == 3)
		socket_server_set_accept_flag(STATUS_NO_ACCEPT);

	socket_t *sk_accept = args;

	log_i("fd: %d", sk_accept->fd);

	socket_client_clean(sk_accept);
}

static void net_v1_server_test(void)
{
	net_v1_server_init(socket_server_cb);
}

static void net_v1_test_init(void)
{
	handle_test_cmd_t net_v1_test_cmd[] = {
		{"13", net_v1_client_test},
		{"14", net_v1_server_test},
	};

	register_test_cmd(net_v1_test_cmd, ARRAY_NUM(net_v1_test_cmd));
}
DECLARE_INIT(net_v1_test_init);

