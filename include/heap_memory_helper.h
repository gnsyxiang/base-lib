/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    heap_memory_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    19/12 2017 11:33
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        19/12 2017      create the file
 * 
 *     last modified: 19/12 2017 11:33
 */
#ifndef _BASE_LIB_HEAP_MEMORY_HELPER_H_
#define _BASE_LIB_HEAP_MEMORY_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_HEAP_MEMORY_HELPER_GB
#define BASE_LIB_HEAP_MEMORY_HELPER_EX extern
#else
#define BASE_LIB_HEAP_MEMORY_HELPER_EX
#endif

BASE_LIB_HEAP_MEMORY_HELPER_EX void _safer_free(void **pp);

#define safer_free(p) _safer_free((void **)&p)

#define 

#ifdef __cplusplus
}
#endif

#endif

