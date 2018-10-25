/**
 * 
 * Release under GPLv2.
 * 
 * @file    timer_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/10 2018 15:55
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/10 2018      create the file
 * 
 *     last modified: 25/10 2018 15:55
 */
#include <stdio.h>

#include "timer.h"

void test1_cb(void *args)
{
    printf("--------1: %s \n", (char *)args);
}

void timer_test(void)
{
    timer_init();

    int timer_handler1 = timer_start(test1_cb, 2000, "timer test");
    int timer_handler2 = timer_start(test1_cb, 1000, "timer test two");

    int ret = timer_cancel(timer_handler1, test1_cb);
    printf("ret: %d \n", ret);
}
