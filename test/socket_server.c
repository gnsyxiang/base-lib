/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    socket_server.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 17:40
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 17:40
 */
#include <stdio.h>

#include "log_helper.h"
#include "parse_cmd.h"
#include "socket_helper.h"
#include "link.h"

static void server_connect_cb(void *user, void *connection)
{
	char buf[256];
	struct socket *sk = connection;

	int len = 0;
	read(sk->fd, &len, 4);
	log_i("len: %d", len);

	read(sk->fd, buf, 16);
	log_i("buf: %s", buf);
}

static int socket_server_test(void)
{
	struct link_ops *link = get_link_ops();
	struct socket *socket = link->create_server("ipc-socket");

	link->wait_for_connect(socket, NULL, server_connect_cb);

	log_i("server test");
	return 0;
}

static int link_server_test(void)
{
	struct link *link = create_link("ipc-link", CONNECTION_SERVER);
	link_wait_for_connect(link, NULL, server_connect_cb);

	return 0;
}

void socket_server_init(void)
{
	handle_test_cmd_t socket_server_test_cmd[] = {
		/*{"2", socket_server_test},*/
		{"2", link_server_test},
	};

	register_test_cmd(socket_server_test_cmd, ARRAY_NUM(socket_server_test_cmd));
}
