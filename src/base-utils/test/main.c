/**
 * 
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    11/07 2019 18:05
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        11/07 2019      create the file
 * 
 *     last modified: 11/07 2019 18:05
 */
#include <stdio.h>

#include "log_wrapper.h"
#include "signal_wrapper.h"

extern int dir_test(void);
extern void cjson_test(void);
extern void log_test(void);
extern void symtab_test(void);
extern void mem_test(void);
extern void list_test(void);
extern void uart_test(void);

int main(int argc, const char *argv[])
{
    register_linux_signal_hanler(argv[0]);

    log_init(LOG_INFO);

    // dir_test();
    // cjson_test();
    // log_test();
    // symtab_test();
    // mem_test();
    // list_test();
    uart_test();

    log_final();

    free_linux_signal_hanler();
    return 0;
}

