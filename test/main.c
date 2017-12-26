/****************************************************************
 * Filename: main.c
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 23/09 2017 11:42
 * Description:
 * 
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	23/09 2017
 ****************************************************************/
#include <stdio.h>

#include "parse_cmd.h"
#include "log_helper.h"

static char usr_input[10];

void memory_test_init(void);
void time_test_init(void);
void wav_test_init(void);

void init(void)
{
	memory_test_init();
	time_test_init();
	wav_test_init();
}

void dis_func(void)
{
	printf("-----------------------------------------------\n");
	printf("enter the sequence number, select the function \n");
	printf("-----------------------------------------------\n");
	printf("1. socket client test \n");
	printf("2. socket server test \n");
	printf("3. heap memory test \n");
	printf("4. time test \n");
	printf("5. wav test \n");
	printf("6. add blank to wav \n");

	printf("input your number: ");
}

int main(int argc, const char *argv[])
{
	dis_func();

	init();

	scanf("%s", usr_input);

	match_test_cmd(usr_input);

    return 0;
}

