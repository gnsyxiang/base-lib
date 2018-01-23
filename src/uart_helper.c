/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    uart_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/12 2017 22:47
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/12 2017      create the file
 * 
 *     last modified: 12/12 2017 22:47
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define UART_HELPER_GB
#include "uart_helper.h"
#undef UART_HELPER_GB

int uart_open(char *port_path)
{
	int fd;

	if (!port_path)
		return -1;

    fd = open(port_path, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd < 0) {
        perror("open serial porterror!\n");
		exit(1);
    }

	if (fcntl(fd, F_GETFL, 0) < 0) {
		log_i("fcntl faild");
		return -1;
	} else {
		log_i("fcntl: %d", fcntl(fd, F_GETFL, 0));
	}

	if (0 == isatty(STDIN_FILENO)) {
		
	}
}

int uart_init(uart_param_t uart_param)
{
}

