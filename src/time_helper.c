/****************************************************************
 * Filename: time_helper.c
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

/*
 * 1. struct timeval {
 *        long tv_sec;    //秒
 *        long tv_usec;   //微妙
 *    }
 * 2. struct timespec {
 *        __time_t tv_sec;  //秒
 *        long int tv_nsec; //纳秒
 *    }
 * 3. struct tm {
 *        int tm_set;       //秒钟 - 取值区间为[0,59]
 *        int tm_min;       //分钟 - 取值区间为[0,59]
 *        int tm_hour;      //时钟 - 取值区间为[0,23]
 *        int tm_mday;      //日期 - 取值区间为[1,31]
 *        int tm_mon;       //月份 - 取值区间为[0,11]
 *        int tm_year;      //年份 - 其值从1900开始
 *        int tm_wday;      //星期 - 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推
 *        int tm_yday;      //天数 - 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推
 *        int tm_isdst;     //夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。
 *    }
 */
#include <stdio.h>

#define TIME_HELPER_GB
#include "time_helper.h"
#undef TIME_HELPER_GB

struct timespec cur_delay_ms(unsigned int timeout_ms)
{
    struct timeval tv;
    struct timespec ts;

    /* returns the current time (sec and usec) */
    gettimeofday(&tv, NULL);

    ts.tv_sec = time(NULL) + timeout_ms / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeout_ms % 1000);

    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    return ts;
}

void get_tm_time(struct tm *tm)
{
    struct timespec ts;

    /* the number of seconds from 1970 to the preset */
    clock_gettime(CLOCK_REALTIME, &ts);

    localtime_r(&ts.tv_sec, tm);
}

void format_time(char *time)
{
    struct tm now_time;

    get_tm_time(&now_time);

    sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d", \
            now_time.tm_year + 1900, now_time.tm_mon + 1, now_time.tm_mday, \
            now_time.tm_hour, now_time.tm_min, now_time.tm_sec);
}

