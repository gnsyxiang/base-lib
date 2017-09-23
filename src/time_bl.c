/****************************************************************
 * Filename: time_bl.c
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 22/09 2017 22:05
 * Description:
 * 
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	22/09 2017
 ****************************************************************/
#include <stdio.h>

#define TIME_BL_GB
#include "time_bl.h"
#undef TIME_BL_GB

struct timespec cur_delay_ms(unsigned int timeout_ms)
{
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);

    ts.tv_sec = time(NULL) + timeout_ms / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeout_ms % 1000);

    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    return ts;
}

