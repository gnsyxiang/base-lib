/**
 *
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
#include <stdio.h>

#include "log_helper.h"
#include "parse_cmd.h"
#include "socket_helper.h"

void socket_client_cb(void *args)
{
	log_i("socket_client_cb ok");
}

static void socket_client(void)
{
	socket_t *sk = socket_client_init(LO_IP, MYPORT);

	socket_connect(sk, socket_client_cb, 3);

	socket_client_clean(sk);

	log_i("socket client test");
}

void socket_client_test_init(void)
{
	handle_test_cmd_t socket_client_test_cmd[] = {
		{"1", socket_client},
	};

	register_test_cmd(socket_client_test_cmd, ARRAY_NUM(socket_client_test_cmd));
}

DECLARE_INIT(socket_client_test_init);

