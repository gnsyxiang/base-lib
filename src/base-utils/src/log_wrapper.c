/**
 * 
 * Release under GPLv2.
 * 
 * @file    log_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/07 2019 16:44
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/07 2019      create the file
 * 
 *     last modified: 08/07 2019 16:44
 */
#include <stdio.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <stdarg.h>
#include <stdlib.h>

#include "time_wrapper.h"

#define BASE_LIB_UTILS_SRC_LOG_WRAPPER_GB
#include "log_wrapper.h"
#undef BASE_LIB_UTILS_SRC_LOG_WRAPPER_GB

typedef struct log_wrapper_tag {
    log_level_t log_level;
}log_wrapper_t;

static log_wrapper_t *log_wrapper_handle;

#define LOG_WRAPPER_LEN         (sizeof(log_wrapper_t))
#define LOG_WRAPPER_TAG         "log_wrapper"

#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*printf("\033[字体高亮;字背景颜色;字体颜色m字符串\033[0m"); */ // \x1b == \033

static char *g_level_2_str[][2] = {
    {ANSI_COLOR_YELLOW, "[A]"},
    {ANSI_COLOR_RED,    "[E]"},
    {ANSI_COLOR_GREEN,  "[W]"},
    {ANSI_COLOR_WHITE,  "[I]"},
    {ANSI_COLOR_WHITE,  "[D]"},
    {ANSI_COLOR_WHITE,  "[V]"},
    {"[N/A]",           "[N/A]"},
};

#define LOG_WRITE_SET_COLOUR(buf, len, ret, level)                     \
    do {                                                               \
        ret += snprintf(buf + ret, len - ret, "%s%s ",                 \
                g_level_2_str[level][0], g_level_2_str[level][1]);     \
    } while(0)

#define LOG_WRITE_RESET_COLOUR(buf, len, ret)                          \
    do {                                                               \
        ret += snprintf(buf + ret, len - ret, "%s", ANSI_COLOR_RESET); \
    } while(0)

static int _kernel_info(char *buf, int len)
{
#define STARTUP_TIME_LEN    (64)
    char startup_time[STARTUP_TIME_LEN];
    int ret = 0;
    int kernel_thread_id = 0;

    kernel_thread_id = syscall(SYS_gettid);
    get_sys_startup_time_2_str(startup_time, STARTUP_TIME_LEN);

    ret += snprintf(buf + ret, len - ret, "%s ", startup_time);
    ret += snprintf(buf + ret, len - ret, "<%d ", kernel_thread_id);

    return ret;
}

static int _app_info(char *buf, int len, char *tag, const char *func, int line)
{
    int ret = 0;

    ret += snprintf(buf + ret, len - ret, "%s> ", tag);
    ret += snprintf(buf + ret, len - ret, "<%s ", func);
    ret += snprintf(buf + ret, len - ret, "%d> ", line);

    return ret;
}

static int _fmt_info(char *buf, int len, char *fmt, va_list args)
{
    int ret = 0;
    ret = vsnprintf(buf, len, fmt, args);
    //FIXME len is too long
    return ret;
}

void log_write(char *tag, log_level_t level, const char *func, int line, int err_no, char *fmt, ...)
{
    int ret = 0;
    char buf[LOG_BUF_SIZE] = {0};
    char *buf_p = buf;
    int len = LOG_BUF_SIZE;

    va_list args;
    va_start(args, fmt);

    LOG_WRITE_SET_COLOUR(buf_p, len, ret, level);

    ret += _kernel_info(buf_p + ret, len - ret);
    ret += _app_info(buf_p + ret, len - ret, tag, func, line);
    ret += _fmt_info(buf_p + ret, len - ret, fmt, args);

    LOG_WRITE_RESET_COLOUR(buf, len, ret);

    va_end(args);

    printf("%s", buf);
}

int log_init(log_level_t log_level)
{
    log_wrapper_handle = calloc(1, LOG_WRAPPER_LEN);
    if (!log_wrapper_handle) {
        loge(LOG_WRAPPER_TAG, "calloc is faild \n");
        return -1;
    }

    log_wrapper_handle->log_level = log_level;

    return 0;
}

void log_final(void)
{
    free(log_wrapper_handle);
    log_wrapper_handle = NULL;
}

void log_set_level(log_level_t log_level)
{
    log_wrapper_handle->log_level = log_level;
}

log_level_t log_get_level(void)
{
    return log_wrapper_handle->log_level;
}

