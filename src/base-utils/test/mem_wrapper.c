/**
 * 
 * Release under GPLv2.
 * 
 * @file    mem_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    24/07 2019 16:40
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        24/07 2019      create the file
 * 
 *     last modified: 24/07 2019 16:40
 */
#include <stdio.h>

#include "mem_wrapper.h"

#define ARR_SIZE 3

void mem_test(void)
{
    void * arr[ARR_SIZE];
    int i;

    for(i = 0; i < ARR_SIZE; i++) {
        arr[i] = NULL;
    }

    for(i = 0; i < ARR_SIZE; i++) {
        arr[i] = mem_calloc(1, sizeof(int));
    }

    for(i = 0; i < ARR_SIZE; i++) {
        mem_free(arr[i]);
    }

    // mem_leak_display();
}

