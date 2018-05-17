/**
 * 
 * Release under GPLv2.
 * 
 * @file    typedef_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/05 2018 19:28
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/05 2018      create the file
 * 
 *     last modified: 17/05 2018 19:28
 */
#include <stdio.h>

#define TYPEDEF_HELPER_GB
#include "typedef_helper.h"
#undef TYPEDEF_HELPER_GB

void check_datatype_mem_len(void)
{
	printf("******************************************\n");
	printf("* sizeof(char):           %d \n", (unsigned int)sizeof(char));
	printf("* sizeof(signed char):    %d \n", (unsigned int)sizeof(unsigned char));
	printf("* sizeof(unsigned char):  %d \n", (unsigned int)sizeof(signed char));
	printf("\n");

	printf("* sizeof(short):          %d \n", (unsigned int)sizeof(short));
	printf("* sizeof(signed short):   %d \n", (unsigned int)sizeof(signed short));
	printf("* sizeof(unsigned short): %d \n", (unsigned int)sizeof(unsigned short));
	printf("\n");

	printf("* sizeof(int):            %d \n", (unsigned int)sizeof(int));
	printf("* sizeof(signed int):     %d \n", (unsigned int)sizeof(signed int));
	printf("* sizeof(unsigned int):   %d \n", (unsigned int)sizeof(unsigned int));
	printf("\n");

	printf("* sizeof(long):           %d \n", (unsigned int)sizeof(long));
	printf("* sizeof(signed long):    %d \n", (unsigned int)sizeof(signed long));
	printf("* sizeof(unsigned long):  %d \n", (unsigned int)sizeof(unsigned long));
	printf("\n");

	printf("* sizeof(float):          %d \n", (unsigned int)sizeof(float));
	printf("* sizeof(double):         %d \n", (unsigned int)sizeof(double));
	printf("* sizeof(long double):    %d \n", (unsigned int)sizeof(long double));
	printf("******************************************\n");
}




