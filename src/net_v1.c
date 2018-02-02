/**
 * 
 * Release under GPLv2.
 * 
 * @file    net_v1.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/02 2018 12:28
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/02 2018      create the file
 * 
 *     last modified: 02/02 2018 12:28
 */
#include <stdio.h>

#define NET_V1_GB
#include "net_v1.h"
#undef NET_V1_GB

void net_v1_client_init(socket_cb_t socket_client_cb)
{
	socket_t *sk = socket_client_init(LO_IP, MYPORT);

	socket_connect(sk, socket_client_cb, 3);

	socket_client_clean(sk);
}

void net_v1_server_init(socket_cb_t socket_server_cb)
{
	socket_t *sk = socket_server_init(MYPORT);

	socket_wait_for_connect(sk, socket_server_cb);

	socket_server_clean(sk);
}

