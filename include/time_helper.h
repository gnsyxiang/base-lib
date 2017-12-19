/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 *
 * @file     time_helper.h
 * @brief    Get the system time and function of operation time.
 * @author   gnsyxiang <gnsyxiang@163.com>
 * @date     2017.09.25
 * @version  0.0.1
 *
 * @since    note
 * @note
 *
 *     Change Log:
 *     NO.      Author              Date		    Modified
 *     00	    zhenquan.qiu	    25/09 2017      create the file
 *
 *     Last modified: 25/09 2017 15:19
 */
#ifndef _TIME_HELPER_H_
#define _TIME_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <sys/time.h>

#include "data_type.h"

#ifndef TIME_HELPER_GB
#define TIME_HELPER_EX extern       /**< control function external reference macro. */
#else
#define TIME_HELPER_EX
#endif

/**
 * return the 'struct timespec' time
 * after the currtent time delay specifies milliseconds.
 *
 * @param timeout_ms: milliseconds requiring delay.
 * @return return the 'struct timespec' time.
 */
TIME_HELPER_EX struct timespec cur_delay_ms(uint32_t timeout_ms);

/**
 * get the current time with 'struct tm'
 *
 * @param tm: pointers to 'struct tm'
 * @return void
 */
TIME_HELPER_EX void get_tm_time(struct tm *tm);

/**
 * get sec from CLOCK_BOOTTIME to now
 *
 * @param void
 * @return return sec from CLOCK_BOOTTIME
 */
TIME_HELPER_EX double get_sec_clk_with_boottime(void);

/**
 * format time as required
 *
 * @param time: a pointer to storing formatting time
 * @return void
 */
TIME_HELPER_EX void format_time(char *time);

/**
 * get the current time of the system in ms
 *
 * @param void
 * @return void
 */
time_t get_sys_time_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* end _TIME_HELPER_H_ */

