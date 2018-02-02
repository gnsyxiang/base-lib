/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    misc_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:31
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:31
 */
#ifndef _MISC_HELPER_H_
#define _MISC_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <linux/swab.h>

#include "data_type.h"

#ifndef MISC_HELPER_GB
#define MISC_HELPER_EX extern
#else
#define MISC_HELPER_EX
#endif

#define FOR_ZERO_TO_LEN(len) for (uint32_t i = 0; i < (len); i++)

// byte order processing function in <linux/swab.h>, eg: __swab16, __swab32
// 内存地址从低地址向高地址增长，big endian是高位数据优先，即高位放在低地址。
//                               little endian是低位数据优先，低位放在低地址。
// 网络字节序采用大端。
static inline uint32_t judgement_byte_order(void)
{
    uint32_t x = 0x01;
    if (0x01 == *(char *)&x) {
        printf("little endian \n");
        return 1;
    } else {
        printf("big endian \n");
        return 0;
    }
}

#if 0
#define swap(a, b)				\
	do {						\
		char tmp;				\
		tmp = a;				\
		a = b;					\
		b = tmp;				\
	} while (0)
#else
#define swap(a, b)				\
	do {						\
		a = a ^ b;				\
		b = a ^ b; 				\
		a = a ^ b; 				\
	} while (0)
#endif
#define is_little_endian()      (1 == judgement_byte_order())
#define is_big_endian()         (0 == judgement_byte_order())



MISC_HELPER_EX void test(void);
MISC_HELPER_EX uint32_t random_num(uint32_t range);

#ifdef __cplusplus
}
#endif

#endif /* end _MISC_HELPER_H_ */

