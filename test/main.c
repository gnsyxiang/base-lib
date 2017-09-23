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

    printf("-1- sec: %ld \n", time(NULL));

    ts = cur_delay_ms(2000);
    printf("-2- sec: %ld \n", ts.tv_sec);
    printf("nsec: %ld \n", ts.tv_nsec);
}

int main(int argc, const char *argv[])
{
    time_test();

    return 0;
}

