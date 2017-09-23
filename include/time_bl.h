/****************************************************************
 * Filename: time_bl.h
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
#ifndef _TIME_BL_H_
#define _TIME_BL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <sys/time.h>

#ifndef TIME_BL_GB
#define TIME_BL_EX extern
#else
#define TIME_BL_EX
#endif


TIME_BL_EX struct timespec cur_delay_ms(unsigned int timeout_ms);
TIME_BL_EX void get_tm_time(struct tm *tm);
TIME_BL_EX void format_time(char *time);

#ifdef __cplusplus
}
#endif

#endif /* end _TIME_BL_H_ */

