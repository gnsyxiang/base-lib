/**
 *
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

#define MAX_BUFFER_SIZE			512

#define SERIAL_SEND_PATH		"/dev/pts/21"
#define SERIAL_RECEIVE_PATH		"/dev/pts/22"

static void uart_recv(void)
{
    char buf[MAX_BUFFER_SIZE], *ptr;
    int ret;

	ptr = buf;

	int fd = uart_open(SERIAL_RECEIVE_PATH);

	uart_init(fd, 9600, 0, 8, 'n', 1);

    while(1) {
		if ((ret = uart_read(fd, ptr, 1)) < 0)
			break;

		printf("%c", *ptr);
    }

	uart_close(fd);

	log_i("receive OK");
}

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

static void uart_test_init(void)
{
	printf(" 7. serial send test \n");
	printf(" 8. serial receive test \n");

	handle_test_cmd_t uart_test_cmd[] = {
		{"7", uart_send},
		{"8", uart_recv},
	};

	register_test_cmd(uart_test_cmd, ARRAY_NUM(uart_test_cmd));
}

DECLARE_INIT(uart_test_init);

