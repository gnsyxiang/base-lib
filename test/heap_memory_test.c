/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    heap_memory_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/12 2017 13:54
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/12 2017      create the file
 * 
 *     last modified: 20/12 2017 13:54
 */
#include <stdio.h>
#include "heap_memory_helper.h"
#include "log_helper.h"
#include "parse_cmd.h"

static int heap_memory_test(void)
{
	char *p = safer_malloc(10);	

	for (int i = 0; i < 10; i++)
		*(p + i) = i;

	for (int i = 0; i < 10; i++)
		log_i("p[%d]: %d ", i, p[i]);

	safer_free(p);

	return 0;
}


void memory_test_init(void)
{
	handle_test_cmd_t memory_test_cmd[] = {
		{"3", heap_memory_test},
	};

	register_test_cmd(memory_test_cmd, ARRAY_NUM(memory_test_cmd));
}

