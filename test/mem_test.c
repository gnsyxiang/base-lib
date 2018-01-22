/**
 *
 * Release under GPLv2.
 * 
 * @file    mem_test.c
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

#include "mem_helper.h"
#include "log_helper.h"
#include "parse_cmd.h"

static void mem_test(void)
{
	char *p = malloc_mem(10);	

	for (int i = 0; i < 10; i++)
		*(p + i) = i;

	for (int i = 0; i < 10; i++)
		log_i("p[%d]: %d ", i, p[i]);

	free_mem(p);
}

static void mem_test_init(void)
{
	handle_test_cmd_t mem_test_cmd[] = {
		{"3", mem_test},
	};

	register_test_cmd(mem_test_cmd, ARRAY_NUM(mem_test_cmd));
}
DECLARE_INIT(mem_test_init);

