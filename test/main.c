/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:39
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:39
 */
#include <stdio.h>

#include "parse_cmd.h"
#include "log_helper.h"

static char usr_input[10];

void dis_func(void)
{
	printf("-----------------------------------------------\n");
	printf("enter the sequence number, select the function \n");
	printf("-----------------------------------------------\n");
	printf(" 1. socket client test \n");
	printf(" 2. socket server test \n");
	printf(" 3. heap memory test \n");
	printf(" 4. time test \n");
	printf(" 5. wav test \n");
	printf(" 6. add blank to wav \n");
	printf(" 7. serial send test \n");
	printf(" 8. serial receive test \n");
	printf(" 9. file test \n");
	printf("10. str test \n");
	printf("11. csv test \n");

	printf("input your number: ");
}

int main(int argc, const char *argv[])
{
	dis_func();

	do_initcalls();

	scanf("%s", usr_input);

	match_test_cmd(usr_input);

    return 0;
}

