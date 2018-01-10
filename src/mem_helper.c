/**
 *
 * Release under GPLv2.
 * 
 * @file    mem_helper.c
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
#include <string.h>

#include "log_helper.h"

#define BASE_LIB_MEM_HELPER_GB
#include "mem_helper.h"
#undef BASE_LIB_MEM_HELPER_GB

void *safer_malloc(size_t len)
{
	void *p = malloc(len);
	if (!p) {
		log_e("malloc faild");
		exit(-1);
	} else {
		memset(p, '\0', len);
		return p;
	}
}

void _safer_free(void **pp)
{
	if (pp != NULL && *pp != NULL) {
		free(*pp);
		*pp = NULL;
	}
}
