/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    misc_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:36
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:36
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define MISC_HELPER_GB
#include "misc_helper.h"
#undef MISC_HELPER_GB

uint32_t random_num(uint32_t range)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

    return (1 + (uint32_t)(1.0 * range * rand() / (RAND_MAX + 1.0)));
}

void test(void)
{
    printf("test \n");
}

