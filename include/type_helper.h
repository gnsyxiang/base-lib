/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    type_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:34
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:34
 */
#ifndef _TYPE_HELPER_H_
#define _TYPE_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "data_type.h"

#ifndef TYPE_HELPER_GB
#define TYPE_HELPER_EX extern
#else
#define TYPE_HELPER_EX
#endif

#if defined _WIN32 || defined __CYGWIN__
    #ifdef BUILDING_DLL
        #ifdef __GNUC__
            #define DLL_PUBLIC __attribute__ ((dllexport))
        #else
            #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
        #endif
    #else
        #ifdef __GNUC__
            #define DLL_PUBLIC __attribute__ ((dllimport))
        #else
            #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
        #endif
    #endif
    #define DLL_LOCAL
#else
    #if __GNUC__ >= 4
        #define DLL_PUBLIC __attribute__ ((visibility ("default")))
        #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define DLL_PUBLIC
        #define DLL_LOCAL
    #endif
#endif

#define BYTE_ALIGN(len, align) (((len) + (align) - 1) & ~((align) - 1))

#define ALIGN_INT(len)  BYTE_ALIGN(len, sizeof(uint32_t))

#define ALIGN4(len)     BYTE_ALIGN(len, 4)
#define ALIGN3(len)     BYTE_ALIGN(len, 3)
#define ALIGN2(len)     BYTE_ALIGN(len, 2)

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)


#ifdef __cplusplus
}
#endif

#endif /* end _TYPE_HELPER_H_ */

