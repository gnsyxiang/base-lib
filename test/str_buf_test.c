/**
 * 
 * Release under GPLv2.
 * 
 * @file    str_buf_test.c
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
#include "str_buf_helper.h"

static void str_buf_test(void)
{
	char buf[] = "abc";
	str_buf_t *str = str_buf_create_by_buf(buf);

	str_buf_dump_buf(str);
	str_buf_insert_char(str, 'd');
	str_buf_dump_buf(str);

	char buf1[] = "efg";
	str_buf_insert_buf(str, buf1);
	str_buf_dump_buf(str);

	str_buf_free(str);
}

static void str_buf_test_init(void)
{
	handle_test_cmd_t str_buf_test_cmd[] = {
		{"10", str_buf_test},
	};

	register_test_cmd(str_buf_test_cmd, ARRAY_NUM(str_buf_test_cmd));
}
DECLARE_INIT(str_buf_test_init);

