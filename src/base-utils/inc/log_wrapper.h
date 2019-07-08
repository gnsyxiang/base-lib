/**
 *
 * Release under GPLv2.
 * 
 * @file    log_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/07 2019 16:35
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/07 2019      create the file
 * 
 *     last modified: 08/07 2019 16:35
 */
#ifndef __BASE_LIB_UTILS_INC_LOG_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_LOG_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>

#ifndef BASE_LIB_UTILS_SRC_LOG_WRAPPER_GB
#define BASE_LIB_UTILS_SRC_LOG_WRAPPER_EX extern
#else
#define BASE_LIB_UTILS_SRC_LOG_WRAPPER_EX
#endif

typedef enum {
    LOG_ASSERT,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_VERBOSE,
    LOG_MAX,
}log_level_t;

#define LOG_BUF_SIZE (1024)

int log_init(log_level_t log_level);
void log_final(void);

void log_set_level(log_level_t log_level);
log_level_t log_get_level(void);

void log_write(char *tag, log_level_t level, const char *func, int line, int err_no, char *fmt, ...);

#define log(tag, level, ...) \
    do { \
        if (level <= log_get_level()) { \
            log_write(tag, level, __func__, __LINE__, errno, __VA_ARGS__); \
        } \
    } while(0)

#define loga(tag, ...) log(tag, LOG_ASSERT,  __VA_ARGS__)
#define loge(tag, ...) log(tag, LOG_ERROR,   __VA_ARGS__)
#define logw(tag, ...) log(tag, LOG_WARN,    __VA_ARGS__)
#define logi(tag, ...) log(tag, LOG_INFO,    __VA_ARGS__)
#define logd(tag, ...) log(tag, LOG_DEBUG,   __VA_ARGS__)
#define logv(tag, ...) log(tag, LOG_VERBOSE, __VA_ARGS__)

#define log_xixi(...) log(LOG_TEST_TAG, LOG_ASSERT,  __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_LOG_WRAPPER_H_ */

