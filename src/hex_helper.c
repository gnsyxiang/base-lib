/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    hex_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/12 2017 22:23
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/12 2017      create the file
 * 
 *     last modified: 08/12 2017 22:23
 */
#include <stdio.h>

#define HEX_HELPER_GB
#include "hex_helper.h"
#undef HEX_HELPER_GB

void print_hex(const unsigned char *buf, int len, int cnt_of_line)
{
	for (int i = 0; i < len; i++) {
		if (i && i % cnt_of_line == 0)
			printf("\n");
		printf("%02x ", buf[i]);
	}
	printf("\n");
}


