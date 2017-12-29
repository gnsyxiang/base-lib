/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    socket_client.c
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

static int socket_client_test(void)
{
	struct link_ops *link = get_link_ops();
	struct socket *socket = link->create_client("ipc-socket");

	link->connect(socket, 4);

	log_i("client test");
	return 0;
}

static int link_client_test(void)
{
	char buf[] = "link client test";

	struct link *link = create_link("ipc-link", CONNECTION_CLIENT);
	link_connect(link);

	link_write(link, buf, sizeof(buf));

	delete_link(link);

	return 0;
}

void socket_client_init(void)
{
	handle_test_cmd_t socket_client_test_cmd[] = {
		/*{"1", socket_client_test},*/
		{"1", link_client_test},
	};

	register_test_cmd(socket_client_test_cmd, ARRAY_NUM(socket_client_test_cmd));
}

