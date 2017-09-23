/****************************************************************
 * Filename: time_helper.h
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 22/09 2017 22:07
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	22/09 2017
 ****************************************************************/
#ifndef _TIME_HELPER_H_
#define _TIME_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <sys/time.h>

#ifndef TIME_HELPER_GB
#define TIME_HELPER_EX extern
#else
#define TIME_HELPER_EX
#endif


TIME_HELPER_EX struct timespec cur_delay_ms(unsigned int timeout_ms);
TIME_HELPER_EX void get_tm_time(struct tm *tm);
TIME_HELPER_EX void format_time(char *time);

#ifdef __cplusplus
}
#endif

#endif /* end _TIME_HELPER_H_ */

