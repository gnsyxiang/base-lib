/****************************************************************
 * Filename: main.c
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 23/09 2017 11:42
 * Description:
 * 
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	23/09 2017
 ****************************************************************/
#include <stdio.h>

#include "time_helper.h"
#include "misc_helper.h"

void time_test(void)
{
    struct timespec ts;
    struct tm now_time;
    char time_buf[256];

    printf("-1- sec: %ld \n", time(NULL));

    ts = cur_delay_ms(2000);
    printf("-2- sec: %ld \n", ts.tv_sec);
    printf("nsec: %ld \n", ts.tv_nsec);

    get_tm_time(&now_time);
    format_time(time_buf);

    printf("time: %s \n", time_buf);

	printf("time_ms: %ld \n", get_sys_time_ms());
}

void random_test(void)
{
    for (int i = 0; i < 100; i++) {
        printf("%d ",  random_num(100));
    }
}

void dis_func(void)
{
	printf("-----------------------------------------------\n");
	printf("enter the sequence number, select the function \n");
	printf("-----------------------------------------------\n");
	printf("1. socket client test \n");
	printf("2. socket server test \n");
	printf("input your number: ");
}

int get_user_input()
{
	int a;

	scanf("%d", &a);

	return a;
}

int socket_client(void);
int socket_server(void);

int main(int argc, const char *argv[])
{
	dis_func();

	switch (get_user_input()) {
		case 1:
			socket_client();
			break;
		case 2:
			socket_server();
			break;
		default:
			break;
	}


    return 0;
}

