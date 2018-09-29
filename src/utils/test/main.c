/**
 * 
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/09 2018 09:42
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/09 2018      create the file
 * 
 *     last modified: 29/09 2018 09:42
 */
#include <stdio.h>
#include <unistd.h>

#include "signal_helper.h"

extern void log_test(void);
extern void list_test(void);

int main(int argc, char const* argv[])
{
    register_linux_signal_hanler(argv[0]);

    list_test();

    while (1) {
        sleep(1);
    }

    return 0;
}

