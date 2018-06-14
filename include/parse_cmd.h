/**
 *
 * Release under GPLv2.
 * 
 * @file    parse_cmd.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/12 2017 16:30
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/12 2017      create the file
 * 
 *     last modified: 20/12 2017 16:30
 */
#ifndef _BASE_LIB_PARSE_CMD_H_
#define _BASE_LIB_PARSE_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_PARSE_CMD_GB
#define BASE_LIB_PARSE_CMD_EX extern
#else
#define BASE_LIB_PARSE_CMD_EX
#endif

#define MAX_TEST_CMD (20)
#define MAX_TEST_CMD_NAME_LEN (256)

#define ARRAY_NUM(array) (sizeof(array) / sizeof(array[0]))

typedef void (*init_call)(void);

#define _init __attribute__((unused, section(".myinit")))
#define DECLARE_INIT(func) init_call _fn_##func _init = func

typedef struct _handle_test_cmd_tag{
	char test_cmd_name[MAX_TEST_CMD_NAME_LEN + 1];
	init_call test_func;
}handle_test_cmd_t;

typedef struct _handle_test_cmd_list_tag {
	int num;
	handle_test_cmd_t handle_test_cmd[MAX_TEST_CMD];
}handle_test_cmd_list_t;

BASE_LIB_PARSE_CMD_EX void do_initcalls(void);
BASE_LIB_PARSE_CMD_EX void match_test_cmd(const char *str);
BASE_LIB_PARSE_CMD_EX void register_test_cmd(handle_test_cmd_t *cmd_array, int num);

#ifdef __cplusplus
}
#endif

#endif

