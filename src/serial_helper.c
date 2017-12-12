/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    serial_helper.c
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

#define SERIAL_HELPER_GB
#include "serial_helper.h"
#undef SERIAL_HELPER_GB

int serial_open(char *port_path)
{
	int fd;

    fd = open(port_path, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(fd == -1) {
        perror("open serial porterror!\n");
		exit(1);
    }

    return fd;
}

