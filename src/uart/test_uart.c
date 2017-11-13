/****************************************************************
 * Filename: test_uart.c
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 13/11 2017 22:38
 * Description:
 * 
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	13/11 2017
 ****************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "uart_helper.h"

enum{
	U_MODE_FULL_422 = 0,
	U_MODE_HALF_485 = 1,
	U_MODE_FULL_232 = 2,
};
#define TIOCSERMODE	0x5460

int main(int argc, char **argv)
{
	int fd = -1;
	char *file = "/dev/ttyS0";

	if( argc > 1 )
	{
		file = argv[1];
	}

	fd = open(file, O_RDWR);
	ioctl(fd, TIOCSERMODE, U_MODE_HALF_485);

	uart_init(fd, 115200, 'n', 8, 1);

	while(1)
	{
		char buf[100] = {0};
		if( read(fd, buf, sizeof(buf)) > 0 )
		{
			printf("buf:%s\n", buf);
			usleep(100000);
			//write(fd, file, strlen(file));
		}
		usleep(1000);
		//write(fd, file, strlen(file));
	}

	return 0;
}
