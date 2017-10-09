/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 *
 * @file    data_type.h
 * @brief   specifying data types is convenient for transplantation
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/10 2017 16:10
 * @version v0.0.1
 *
 * @since    note
 * @note     note
 *
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/10 2017      create the file
 *
 *     last modified: 09/10 2017 16:10
 */
#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <linux/types.h>

typedef		unsigned char   __u8;
typedef		signed char     __s8;
typedef		unsigned short  __u16;
typedef		signed short    __s16;
typedef		unsigned int    __u32;
typedef		signed int      __s32;

typedef		__u8		uint8_t;
typedef		__s8		int8_t;
typedef		__u16		uint16_t;
typedef		__s16		int16_t;
typedef		__u32		uint32_t;
typedef		__s32		int32_t;

#ifdef __cplusplus
}
#endif

#endif /* end _DATA_TYPE_H_ */

