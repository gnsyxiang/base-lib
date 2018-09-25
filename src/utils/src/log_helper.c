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
#include <string.h>

#include "time_helper.h"

#define BASE_LIB_LOG_GB
#include "log_helper.h"
#undef BASE_LIB_LOG_GB

#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*printf("\033[字体高亮;字背景颜色;字体颜色m字符串\033[0m"); */

#define LOG_NAME_LEN    256
#define LOG_BUF_SIZE    2048

static int log_debug_level = LOG_VERBOSE;
static int log_save_to_file_flag = 0;
static FILE *log_file_fp;
static char log_file_path[LOG_NAME_LEN];

static char pri_buf[LOG_BUF_SIZE] = {0};
static char log_buf[LOG_BUF_SIZE] = {0};

static void log_output(const char *pri_buf, const char *log_buf, int len)
{
    if (log_save_to_file_flag) {
        fwrite(log_buf, len, 1, log_file_fp);
        fflush(log_file_fp);
    }

    printf("%s", pri_buf);
}

static int _log_debug(int type, int level, int num, 
        char * const fmt_buf, char * const buf)
{
    int len = 0;

	if (level <= LOG_ERROR && type)
		len += snprintf(buf + len, LOG_BUF_SIZE - len, ANSI_COLOR_RED);

    len +=  sprintf(buf + len, "%s", fmt_buf);

	if (level <= LOG_ERROR) {
		len +=  sprintf(buf + len, " [errno string: %s]", strerror(num));
        if (type)
            len += snprintf(buf + len, LOG_BUF_SIZE - len, ANSI_COLOR_RESET);
	}

    len += sprintf(buf + len, "\n");

    return len;
}

void log_debug(int level, const char *file, int line, int num, const char *fmt, ...)
{
    int len = 0;

    if (!((level <= LOG_VERBOSE) && (level <= log_debug_level)))
        return ;

    memset(pri_buf, '\0', LOG_BUF_SIZE);
    memset(log_buf, '\0', LOG_BUF_SIZE);

           sprintf(log_buf,       "[%.03f]",    get_sec_clk_with_boottime());
    len += sprintf(pri_buf,       "[%.03f]",    get_sec_clk_with_boottime());

           sprintf(log_buf + len, "[%s +%d]: ", file, line);
    len += sprintf(pri_buf + len, "[%s +%d]: ", file, line);

    char fmt_buf[LOG_BUF_SIZE] = {0};
    va_list var_args;

    va_start(var_args, fmt);
    vsnprintf(fmt_buf, LOG_BUF_SIZE, fmt, var_args);
    va_end(var_args);

           _log_debug(1, level, num, fmt_buf, pri_buf + len);
    len += _log_debug(0, level, num, fmt_buf, log_buf + len);

    log_output(pri_buf, log_buf, len);
}

int log_init(const char *name, int level, int save_flag)
{
    if (strlen(name) +1 >= LOG_NAME_LEN) {
        printf("the log file name is too long \n");
        return -1;
    }

    strncpy(log_file_path, name, LOG_NAME_LEN - 1);
    log_file_path[LOG_NAME_LEN] = '\0';

    if (!name)
        save_flag = 0;

    if (save_flag)
        log_file_fp = fopen(log_file_path, "wb+");
    
    log_debug_level = level;
    log_save_to_file_flag = save_flag;

    return 0;
}

