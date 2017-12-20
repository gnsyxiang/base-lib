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
#include <string.h>

#include "time_helper.h"
#include "misc_helper.h"
#include "log_helper.h"
#include "parse_cmd.h"

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
	printf("3. heap memory test \n");
	printf("input your number: ");
}

int get_user_input()
{
	int a;

	scanf("%d", &a);

	return a;
}

void memory_test_init(void);

int main(int argc, const char *argv[])
{
	dis_func();

	memory_test_init();

	match_test_cmd("3");
	/*match_test_cmd(get_user_input() + '0');*/

    return 0;
}

