/****************************************************************
 * Filename: misc_helper.h
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 22/09 2017 21:54
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	22/09 2017
 ****************************************************************/

#ifndef _MISC_HELPER_H_
#define _MISC_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <linux/swab.h>

#ifndef MISC_HELPER_GB
#define MISC_HELPER_EX extern
#else
#define MISC_HELPER_EX
#endif

#define FOR_ZERO_TO_LEN(len) for (int i = 0; i < (len); i++)

// byte order processing function in <linux/swab.h>, eg: __swab16, __swab32
// 内存地址从低地址向高地址增长，big endian是高位数据优先，即高位放在低地址。
//                               little endian是低位数据优先，低位放在低地址。
// 网络字节序采用大端。
static inline int judgement_byte_order(void)
{
    int x = 0x01;
    if (0x01 == *(char *)&x) {
        printf("little endian \n");
        return 1;
    } else {
        printf("big endian \n");
        return 0;
    }
}
#define is_little_endian()      (1 == judgement_byte_order())
#define is_big_endian()         (0 == judgement_byte_order())



MISC_HELPER_EX void test(void);
MISC_HELPER_EX int random_num(int range);

#ifdef __cplusplus
}
#endif

#endif /* end _MISC_HELPER_H_ */

