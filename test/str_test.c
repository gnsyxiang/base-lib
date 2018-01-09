/**
 * 
 * Release under GPLv2.
 * 
 * @file    str_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/01 2018 15:27
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/01 2018      create the file
 * 
 *     last modified: 09/01 2018 15:27
 */
#include <stdio.h>

#include "log_helper.h"
#include "parse_cmd.h"
#include "str_helper.h"

static int str_test(void)
{
	char buf[] = "abc";
	str_t *str = str_create_by_buf(buf);

	str_dump(str);
	str_insert_char(str, 'd');
	str_dump(str);

	char buf1[] = "efg";
	str_insert_buf(str, buf1);
	str_dump(str);

	str_free(str);

	return 0;
}

void str_test_init(void)
{
	handle_test_cmd_t str_test_cmd[] = {
		{"10", str_test},
	};

	register_test_cmd(str_test_cmd, ARRAY_NUM(str_test_cmd));
}

