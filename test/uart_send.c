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

int fd,flag_close;

static void uart_send(void)
{
    char sbuf[] = {"hello world \n"};
    int retv;

	fd = uart_open(SERIAL_SEND_PATH);

    printf("Ready for sendingdata...\n");

	uart_init(fd, 9600, 0, 8, 'n', 1);

    int count = 0;
    int length =sizeof(sbuf);
    while (count++ < 10) {
        sleep(1);

        retv = write(fd, sbuf,length);
        if(retv == -1)
        {
            perror("Write dataerror!\n");
            return ;
        }
    }

	uart_close(fd);
}

static void uart_send_init(void)
{
	handle_test_cmd_t uart_send_test_cmd[] = {
		{"7", uart_send},
	};

	register_test_cmd(uart_send_test_cmd, ARRAY_NUM(uart_send_test_cmd));
}

DECLARE_INIT(uart_send_init);

