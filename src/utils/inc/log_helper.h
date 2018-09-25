/**
 *
 * Release under GPLv2.
 * 
 * @file    log_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    19/12 2017 14:01
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        19/12 2017      create the file
 * 
 *     last modified: 19/12 2017 14:01
 */
#ifndef __BASE_LIB_LOG_H_
#define __BASE_LIB_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>

#ifndef BASE_LIB_LOG_GB
#define BASE_LIB_LOG_EX extern
#else
#define BASE_LIB_LOG_EX
#endif

enum {
    LOG_FATAL = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_VERBOSE,
};

BASE_LIB_LOG_EX void log_debug(int level, const char *file, int line, int num, const char *fmt, ...);

/*
#define LOG_DEBUG(level, ...) \
	log_debug(level, basename(__FILE__), __LINE__, __VA_ARGS__)
*/

#define LOG_DEBUG(level, ...) \
	log_debug(level, __FILE__, __LINE__, errno,  __VA_ARGS__)

#define log_d(...)      LOG_DEBUG(LOG_DEBUG,	__VA_ARGS__)
#define log_i(...)      LOG_DEBUG(LOG_INFO,		__VA_ARGS__)
#define log_w(...)      LOG_DEBUG(LOG_WARN,		__VA_ARGS__)
#define log_e(...)      LOG_DEBUG(LOG_ERROR,	__VA_ARGS__)

int log_init(const char *name, int level, int save_flag);

#ifdef __cplusplus
}
#endif

#endif

