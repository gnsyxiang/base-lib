/**
 * Copyright (C) 2017 xxx Co., Ltd.
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

#include "time_helper.h"

#define BASE_LIB_LOG_GB
#include "log_helper.h"
#undef BASE_LIB_LOG_GB

#define LOG_BUF_SIZE 2048

static int log_debug_level = LOG_VERBOSE;

static inline void log_output(char *buffer)
{
    printf("%s\n", buffer);
}

static inline void log_output_n(char *buffer)
{
    printf("%s", buffer);
}

void log_debug(int new_line, int level, const char *file, int line, const char *fmt, ...)
{
    char buffer[LOG_BUF_SIZE];
    int size = 0, sz;

    if (!((level <= LOG_VERBOSE) && (level <= log_debug_level)))
        return ;

    size = sprintf(buffer, "[%.03f]", get_sec_clk_with_boottime());

    sz = sprintf(buffer + size, "[%s +%d]: ", file, line);
    size += sz;

    va_list var_args;

    va_start(var_args, fmt);
    vsnprintf(buffer + size, LOG_BUF_SIZE - size, fmt, var_args);
    va_end(var_args);

	if (new_line)
		log_output(buffer);
	else
		log_output_n(buffer);

}

void log_set_debug_level(int level)
{
    log_debug_level = level;
}

