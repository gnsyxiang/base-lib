/**
 *
 * Release under GPLv2.
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
#ifndef __UTILS_TIME_HELPER_H_
#define __UTILS_TIME_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <sys/time.h>

#ifndef UTILS_TIMER_GB
#define UTILS_TIMER_EX extern
#else
#define UTILS_TIMER_EX
#endif

/**
 * return the 'struct timespec' time
 * after the currtent time delay specifies milliseconds.
 *
 * @param timeout_ms: milliseconds requiring delay.
 * @return return the 'struct timespec' time.
 */
UTILS_TIMER_EX struct timespec cur_delay_ms(unsigned int timeout_ms);

/**
 * get the current time with 'struct tm'
 *
 * @param tm: pointers to 'struct tm'
 * @return void
 */
UTILS_TIMER_EX void get_tm_time(struct tm *tm);

/**
 * get sec from CLOCK_BOOTTIME to now
 *
 * @param void
 * @return return sec from CLOCK_BOOTTIME
 */
UTILS_TIMER_EX double get_sec_clk_with_boottime(void);

/**
 * format time as required
 *
 * @param time: a pointer to storing formatting time
 * @return void
 */
UTILS_TIMER_EX void format_time(char *time);

/**
 * get the current time of the system in ms
 *
 * @param void
 * @return void
 */
UTILS_TIMER_EX time_t get_sys_time_ms(void);

UTILS_TIMER_EX void time_ms_to_timeval(size_t time_ms, 
        struct timeval * const time);

UTILS_TIMER_EX void disp_timeval(const struct timeval *timeval, 
        const char *info);

#ifdef __cplusplus
}
#endif

#endif /* end __UTILS_TIME_HELPER_H_ */

