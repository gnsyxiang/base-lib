/**
 * 
 * Release under GPLv2.
 * 
 * @file    demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/07 2018 14:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/07 2018      create the file
 * 
 *     last modified: 25/07 2018 14:20
 */
#include <stdio.h>

#include "demo.h"

void demo_init(void)
{
	printf("%s init \n", LIB_SO_NMAE);
}

void demo_clean(void)
{
	printf("%s clean \n", LIB_SO_NMAE);
}

