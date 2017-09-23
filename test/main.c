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

#include "time_bl.h"

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
}

int main(int argc, const char *argv[])
{
    time_test();

    return 0;
}

