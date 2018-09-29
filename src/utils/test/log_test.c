/**
 * 
 * Release under GPLv2.
 * 
 * @file    log_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/09 2018 09:50
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/09 2018      create the file
 * 
 *     last modified: 29/09 2018 09:50
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log_helper.h"

void log_test(void)
{
    log_init("res/log.log", LOG_VERBOSE, 1);

    int cnt = 0;
    while (1) {
        if (cnt++ > 15)
            break;

        log_i("cnt: %d", cnt);
        log_e("cnt: %d", cnt);
    }

    int fd = open("haha.log", O_RDONLY);
    if (fd < 0)
        log_e("open faild");
}

