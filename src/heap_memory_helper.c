/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    heap_memory_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    19/12 2017 11:24
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        19/12 2017      create the file
 * 
 *     last modified: 19/12 2017 11:24
 */
#include <stdio.h>
#include <stdlib.h>

#include "log_helper.h"

#define BASE_LIB_HEAP_MEMORY_HELPER_GB
#include "heap_memory_helper.h"
#undef BASE_LIB_HEAP_MEMORY_HELPER_GB

void *safer_malloc(size_t size)
{
	void *pi = malloc(size);
	if (!pi) {
		log_e("malloc faild");
		exit(-1);
	} else
		return pi;
}

void _safer_free(void **pp)
{
	if (pp != NULL && *pp != NULL) {
		free(*pp);
		*pp = NULL;
	}
}
