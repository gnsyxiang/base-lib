/**
 * 
 * Release under GPLv2.
 * 
 * @file    socket-test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/09 2018 09:23
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/09 2018      create the file
 * 
 *     last modified: 28/09 2018 09:23
 */
#include <stdio.h>

#include <utils/log_helper.h>
#include <utils/parse_cmd.h>

#include "socket.h"

// #define DEF_NTP_SERVER "www.baidu.com"
#define DEF_NTP_SERVER "ntp1.aliyun.com"

void socket_test(void)
{
    char ip[INET_ADDRSTRLEN] = {0};

    hostname_to_ip(DEF_NTP_SERVER, ip);

    log_i("ip: %s", ip);
}

static void socket_init(void)
{
	printf("6. socket init\n");

	handle_test_cmd_t socket_cmd[] = {
		{"6", socket_test},
	};

	register_test_cmd(socket_cmd, ARRAY_NUM(socket_cmd));
}
DECLARE_INIT(socket_init);

