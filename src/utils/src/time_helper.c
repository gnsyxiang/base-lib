/**
 *
 * Release under GPLv2.
 * 
 * @file    time_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:38
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:38
 */

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

#define BASE_NUM	(1000)
#define BASE_NUM_2	(1000 * 1000)
#define BASE_NUM_3	(1000 * 1000 * 1000)

#define take_integet_1000(num)		((num) / BASE_NUM)
#define take_remainder_1000(num)	((num) % BASE_NUM)
#define take_multiplier_1000(num)	((num) * BASE_NUM)

time_t get_sys_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return (take_multiplier_1000(tv.tv_sec) + take_integet_1000(tv.tv_usec));
}

struct timespec cur_delay_ms(bl_uint32_t timeout_ms)
{
    struct timeval tv;
    struct timespec ts;

    /* returns the current time (sec and usec) */
    gettimeofday(&tv, NULL);

    ts.tv_sec = time(NULL) + take_integet_1000(timeout_ms);
    ts.tv_nsec = take_multiplier_1000(tv.tv_usec) + BASE_NUM_2 * take_remainder_1000(timeout_ms);

    ts.tv_sec += ts.tv_nsec / BASE_NUM_3;
    ts.tv_nsec %= BASE_NUM_3;

    return ts;
}

void time_ms_to_timeval(size_t time_ms, struct timeval * const time)
{
	time->tv_sec = take_integet_1000(time_ms);
	time->tv_usec = take_multiplier_1000(take_remainder_1000(time_ms));
}

/* int clock_gettime(clockid_t clk_id, struct timespec *tp);
 * param: clockid_t
 *        CLOCK_REALTIME: 实际时间, 用户可以通过date或系统调用去修改, 系统休眠，仍然会运行(系统恢复时，kernel去作补偿)
 *        CLOCK_MONTONIC: 单调时间，从某个时间开始到现在过去的时间。用户不能修改这个时间，系统休眠, 该时间不会增加
 *        CLOCK_MONOTONIC_RAW: 
 *        CLOCK_BOOTTIME: 与CLOCK_MONOTONIC类似，但是当suspend时，会依然增加
 */

double get_sec_clk_with_boottime(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_BOOTTIME, &ts);
    return (double) ts.tv_sec + (double) (ts.tv_nsec) / BASE_NUM_3;
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

