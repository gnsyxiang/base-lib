/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    uart_send.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/12 2017 23:02
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/12 2017      create the file
 * 
 *     last modified: 12/12 2017 23:02
 */
#include <stdio.h>
#include <unistd.h>

#include "uart_helper.h"
#include "log_helper.h"
#include "parse_cmd.h"

#define MAX_BUFFER_SIZE512
#define SERIAL_SEND_PATH "/dev/pts/21"

static void uart_send(void)
{
    char buf[] = {"hello world\n"};
    int ret;
	int cnt = 0;

	int fd = uart_open(SERIAL_SEND_PATH);

	uart_init(fd, 9600, 0, 8, 'n', 1);

    while (cnt++ < 5) {
        sleep(1);

		if ((ret = uart_write(fd, buf, sizeof(buf))) < 0)
			break;
    }

	uart_close(fd);

	log_i("send OK");
}

static void uart_send_init(void)
{
	handle_test_cmd_t uart_send_test_cmd[] = {
		{"7", uart_send},
	};

	register_test_cmd(uart_send_test_cmd, ARRAY_NUM(uart_send_test_cmd));
}

DECLARE_INIT(uart_send_init);

