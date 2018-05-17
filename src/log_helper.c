/**
 *
 * Release under GPLv2.
 * 
 * @file    log_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    19/12 2017 14:03
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        19/12 2017      create the file
 * 
 *     last modified: 19/12 2017 14:03
 */
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <libgen.h>

#include "time_helper.h"

#define BASE_LIB_LOG_GB
#include "log_helper.h"
#undef BASE_LIB_LOG_GB

#define LOG_BUF_SIZE 2048

#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*printf("\033[字体高亮;字背景颜色;字体颜色m字符串\033[0m"); */

static int log_debug_level = LOG_VERBOSE;

static void log_output(char *buffer)
{
    printf("%s\n", buffer);
}

void log_debug(int level, const char *file, int line, const char *fmt, ...)
{
    char buffer[LOG_BUF_SIZE] = {0};
    int size = 0;

    if (!((level <= LOG_VERBOSE) && (level <= log_debug_level)))
        return ;

	if (level <= LOG_ERROR)
		size += snprintf(buffer + size, LOG_BUF_SIZE - size, ANSI_COLOR_MAGENTA);
		/*size += snprintf(buffer + size, LOG_BUF_SIZE - size, "\033[35m");*/

    size = sprintf(buffer, "[%.03f]", get_sec_clk_with_boottime());

    size += sprintf(buffer + size, "[%s +%d]: ", file, line);

	if (level <= LOG_ERROR)
		size += snprintf(buffer + size, LOG_BUF_SIZE - size, ANSI_COLOR_RED);

    va_list var_args;

    va_start(var_args, fmt);
    size += vsnprintf(buffer + size, LOG_BUF_SIZE - size, fmt, var_args);
    va_end(var_args);

	if (level <= LOG_ERROR)
		size += snprintf(buffer + size, LOG_BUF_SIZE - size, "\033[0m");

    log_output(buffer);
}

void log_set_debug_level(int level)
{
    log_debug_level = level;
}

