/**
 * 
 * Release under GPLv2.
 * 
 * @file    str_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/01 2018 10:14
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/01 2018      create the file
 * 
 *     last modified: 26/01 2018 10:14
 */
#include <stdio.h>
#include <string.h>

#include "log_helper.h"
#include "parse_cmd.h"
#include "str_helper.h"

static void str_test(void)
{
	char ext_name[50];
	char *file_name = "/home/os/samba/test.wav";

	str_get_file_extension_name(file_name, ext_name);
	
	printf("file_name: %s, ext_name: %s \n", file_name, ext_name);
}


static void strcmp_hook_test(void)
{
	char *hook_test = "test";

	if(strcmp(hook_test, "test"))
		printf("Incorrect password\n");
	else
		printf("Correct password\n");
}

static void str_test_init(void)
{
	handle_test_cmd_t str_test_cmd[] = {
		/*{"10", str_test},*/
		{"10", strcmp_hook_test},
	};

	register_test_cmd(str_test_cmd, ARRAY_NUM(str_test_cmd));
}
DECLARE_INIT(str_test_init);

