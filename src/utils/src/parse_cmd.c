/**
 *
 * Release under GPLv2.
 * 
 * @file    parse_cmd.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/12 2017 16:39
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/12 2017      create the file
 * 
 *     last modified: 20/12 2017 16:39
 */
#include <stdio.h>
#include <string.h>

#include "log_helper.h"

#define PARSE_CMD_GB
#include "parse_cmd.h"
#undef PARSE_CMD_GB

static handle_test_cmd_list_t handle_test_cmd_list;

extern init_call _init_start;
extern init_call _init_end;

void do_initcalls(void)
{
	init_call *init_ptr = &_init_start;
	for (; init_ptr < &_init_end; init_ptr++) {
		/*printf("init address: %p\n", init_ptr);*/
		(*init_ptr)();
	}
}

void match_test_cmd(const char *str)
{
	if (strlen(str) > MAX_TEST_CMD_NAME_LEN) {
		log_e("str is too long ");
		return;
	} 

	for (int i = 0; i < handle_test_cmd_list.num; i++) {
		if (!strcmp(handle_test_cmd_list.handle_test_cmd[i].test_cmd_name, str))
			handle_test_cmd_list.handle_test_cmd[i].test_func();
	}
}

void register_test_cmd(handle_test_cmd_t *cmd_array, int num)
{
	if (num > MAX_TEST_CMD - handle_test_cmd_list.num) {
		log_e("cmd array is full");
		return;
	}

	for (int i = 0; i < num; i++) {
		strcpy(handle_test_cmd_list.handle_test_cmd[handle_test_cmd_list.num].test_cmd_name, cmd_array[i].test_cmd_name);
		handle_test_cmd_list.handle_test_cmd[handle_test_cmd_list.num].test_func = cmd_array[i].test_func;
		handle_test_cmd_list.num++;
	}
}

