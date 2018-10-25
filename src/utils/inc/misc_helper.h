/**
 *
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
#ifndef __UTILS_MISC_HELPER_H_
#define __UTILS_MISC_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <linux/swab.h>

#include "typedef_helper.h"

#ifndef UTILS_MISC_GB
#define UTILS_MISC_EX extern
#else
#define UTILS_MISC_EX
#endif

#define FOR_ZERO_TO_LEN(len) for (bl_uint32_t i = 0; i < (len); i++)

// byte order processing function in <linux/swab.h>, eg: __swab16, __swab32
// 内存地址从低地址向高地址增长，big endian是高位数据优先，即高位放在低地址。
//                               little endian是低位数据优先，低位放在低地址。
// 网络字节序采用大端。
static inline bl_uint32_t judgement_byte_order(void)
{
    bl_uint32_t x = 0x01;
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



UTILS_MISC_EX void test(void);
UTILS_MISC_EX bl_uint32_t random_num(bl_uint32_t range);

#ifdef __cplusplus
}
#endif

#endif /* end _MISC_HELPER_H_ */

