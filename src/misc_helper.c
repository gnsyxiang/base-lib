/****************************************************************
 * Filename: misc_helper.c
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 22/09 2017 21:54
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	22/09 2017
 ****************************************************************/
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

