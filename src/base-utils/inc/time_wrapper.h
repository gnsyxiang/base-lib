/**
 *
 * Release under GPLv2.
 * 
 * @file    time_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    19/07 2019 17:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        19/07 2019      create the file
 * 
 *     last modified: 19/07 2019 17:48
 */
#ifndef __BASE_LIB_UTILS_INC_TIME_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_TIME_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_UTILS_SRC_TIME_WRAPPER_GB
#define BASE_LIB_UTILS_SRC_TIME_WRAPPER_EX extern
#else
#define BASE_LIB_UTILS_SRC_TIME_WRAPPER_EX
#endif

#include <time.h>
#include <sys/time.h>

/**
 * return the 'struct timespec' time
 * after the currtent time delay specifies milliseconds.
 *
 * @param timeout_ms: milliseconds requiring delay.
 * @return return the 'struct timespec' time.
 */
struct timespec cur_delay_ms(unsigned int timeout_ms);

/**
 * get the current time with 'struct tm'
 *
 * @param tm: pointers to 'struct tm'
 * @return void
 */
void get_tm_time(struct tm *tm);

/**
 * get sec from CLOCK_BOOTTIME to now
 *
 * @param void
 * @return return sec from CLOCK_BOOTTIME
 */
double get_sec_clk_with_boottime(void);

/**
 * format time as required
 *
 * @param time: a pointer to storing formatting time
 * @return void
 */
void format_time(char *time);

/**
 * get the current time of the system in ms
 *
 * @param void
 * @return void
 */
time_t get_sys_time_ms(void);

void time_ms_to_timeval(size_t time_ms, struct timeval * const time);

void disp_timeval(const struct timeval *timeval, const char *info);

void get_sys_startup_time_2_str(char *buf, int len);

#ifdef __cplusplus
}
#endif

#endif /* end __BASE_LIB_UTILS_INC_TIME_WRAPPER_H_ */

