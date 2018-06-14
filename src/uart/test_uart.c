/**
 * 
 * Release under GPLv2.
 * 
 * @file    test_uart.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/06 2018 10:01
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/06 2018      create the file
 * 
 *     last modified: 14/06 2018 10:01
 */
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
