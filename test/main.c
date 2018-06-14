/**
 *
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
	printf("input your number: ");

	fflush(stdout);
}

int main(int argc, const char *argv[])
{
	do_initcalls();

	dis_func();

	scanf("%s", usr_input);

	match_test_cmd(usr_input);

    return 0;
}

