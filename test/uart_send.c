/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
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
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include <stdlib.h>

#include "uart_helper.h"

#define MAX_BUFFER_SIZE512
#define SERIAL_SEND_PATH "/dev/pts/20"

int fd,flag_close;

int serial_send(void)
{
    char sbuf[] = {"hello world \n"};
    int retv;
    struct termios option;

	fd = uart_init(SERIAL_SEND_PATH);
    if(fd < 0) {
		printf("serial open faild \n");
		exit(1);
    }

    printf("Ready for sendingdata...\n");

    tcgetattr(fd,&option);
    cfmakeraw(&option);

    cfsetispeed(&option,B9600);
    cfsetospeed(&option,B9600);

    tcsetattr(fd, TCSANOW,&option);

    int count = 0;
    int length =sizeof(sbuf);
    while (count++ < 10) {
        sleep(1);

        retv = write(fd, sbuf,length);
        if(retv == -1)
        {
            perror("Write dataerror!\n");
            return -1;
        }
    }

    close(fd);

    printf("The number of charsent is %d\n", retv);
    return 0;
}
