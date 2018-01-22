/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    datatype_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/12 2017 17:07
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/12 2017      create the file
 * 
 *     last modified: 07/12 2017 17:07
 */
#include <stdio.h>

#define DATATYPE_GB
#include "datatype_helper.h"
#undef DATATYPE_GB

void check_datatype_mem_len(void)
{
	printf("******************************************\n");
	printf("* sizeof(char):           %ld \n", sizeof(char));
	printf("* sizeof(signed char):    %ld \n", sizeof(unsigned char));
	printf("* sizeof(unsigned char):  %ld \n", sizeof(signed char));
	printf("\n");

	printf("* sizeof(short):          %ld \n", sizeof(short));
	printf("* sizeof(signed short):   %ld \n", sizeof(signed short));
	printf("* sizeof(unsigned short): %ld \n", sizeof(unsigned short));
	printf("\n");

	printf("* sizeof(int):            %ld \n", sizeof(int));
	printf("* sizeof(signed int):     %ld \n", sizeof(signed int));
	printf("* sizeof(unsigned int):   %ld \n", sizeof(unsigned int));
	printf("\n");

	printf("* sizeof(long):           %ld \n", sizeof(long));
	printf("* sizeof(signed long):    %ld \n", sizeof(signed long));
	printf("* sizeof(unsigned long):  %ld \n", sizeof(unsigned long));
	printf("\n");

	printf("* sizeof(float):          %ld \n", sizeof(float));
	printf("* sizeof(double):         %ld \n", sizeof(double));
	printf("* sizeof(long double):    %ld \n", sizeof(long double));
	printf("******************************************\n");
}




