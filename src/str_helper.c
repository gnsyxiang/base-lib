/**
 * 
 * Release under GPLv2.
 * 
 * @file    str_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/01 2018 09:30
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/01 2018      create the file
 * 
 *     last modified: 26/01 2018 09:30
 */
#include <stdio.h>
#include <string.h>

#include "misc_helper.h"

#define STR_HELPER_GB
#include "str_helper.h"
#undef STR_HELPER_GB

void str_swap_first_and_end(char *str)
{
	int len = strlen(str);
	int cnt = 0;

	while (cnt < len / 2) {
		swap(str[cnt], str[len - cnt - 1]);
		cnt++;
	}
}

void str_get_file_extension_name(const char *file_name, char *ext_name)  
{  
	const char *p = file_name + strlen(file_name) - 1;
	char *pext_name = ext_name;

	while (*p != '.')
		*ext_name++ = *p--;

	*ext_name = '\0';

	str_swap_first_and_end(pext_name);
}  





