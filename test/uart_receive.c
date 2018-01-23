/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    uart_receive.c
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

#define MAX_BUFFER_SIZE 512

int fd,s;

#define SERIAL_RECEIVE_PATH "/dev/pts/22"

static void uart_recv(void)
{
    char hd[MAX_BUFFER_SIZE],*rbuf;
    int retv;

	fd = uart_open(SERIAL_RECEIVE_PATH);

	uart_init(fd, 9600, 0, 8, 'n', 1);

    rbuf = hd;
    printf("Ready for receivingdata...\n");

    while(1)
    {
        while((retv = read(fd,rbuf, 1)) > 0)
            printf("%c",*rbuf);
    }

	uart_close(fd);
}

static void uart_recv_init(void)
{
	handle_test_cmd_t uart_recv_test_cmd[] = {
		{"8", uart_recv},
	};

	register_test_cmd(uart_recv_test_cmd, ARRAY_NUM(uart_recv_test_cmd));
}

DECLARE_INIT(uart_recv_init);

