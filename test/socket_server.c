/**
 *
 * Release under GPLv2.
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

#include "log_helper.h"
#include "parse_cmd.h"
#include "socket_helper.h"

void socket_server_cb(void *args)
{
	log_i("socket_server_cb ok");
}

static void socket_server(void)
{
	socket_t *sk = socket_server_init(MYPORT);

	socket_wait_for_connect(sk, socket_server_cb);

	socket_server_clean(sk);
	
	log_i("socket server test");
}

void socket_server_test_init(void)
{
	handle_test_cmd_t socket_server_test_cmd[] = {
		{"2", socket_server},
	};

	register_test_cmd(socket_server_test_cmd, ARRAY_NUM(socket_server_test_cmd));
}

DECLARE_INIT(socket_server_test_init);

