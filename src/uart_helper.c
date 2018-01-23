/**
 *
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
#include <unistd.h>

#include "log_helper.h"

#define UART_HELPER_GB
#include "uart_helper.h"
#undef UART_HELPER_GB

int uart_open(const char *path)
{
	int fd;

	if (!path)
		return -1;

    fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd < 0) {
        perror("open serial porterror!\n");
		return -1;
    }

	if (fcntl(fd, F_GETFL, 0) < 0) {
		log_i("fcntl faild");
		return -1;
	}

	return fd;
}

void uart_close(int fd)
{
	close(fd);
}


