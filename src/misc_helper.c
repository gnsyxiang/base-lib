/**
 *
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

/* 栈的开口方向：
 * 开口向下的栈先分配大地址，从下到上地址编号依次变大
 * 开口向上的栈先分配小地址
 *
 * 一般情况下，操作系统提供的栈都是向下的
 *
 * 不管栈的开口朝向，数组buf的内存地址buf+1永远向上
 */
void stacks_detection(void)
{
	int a;
	int b;
	char buf[3];

	printf("%p \n", &a);
	printf("%p \n", &b);

	for (int i = 0; i < 3; i++)
		printf("%p \n", &buf[i]);
}

bl_uint32_t random_num(bl_uint32_t range)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

    return (1 + (bl_uint32_t)(1.0 * range * rand() / (RAND_MAX + 1.0)));
}

void test(void)
{
    printf("test \n");
}

