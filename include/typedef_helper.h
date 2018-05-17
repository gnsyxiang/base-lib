/**
 * 
 * Release under GPLv2.
 * 
 * @file    typedef_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/05 2018 19:25
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/05 2018      create the file
 * 
 *     last modified: 17/05 2018 19:25
 */

#ifndef __BASE_LIB_TYPEDEF_HELPER_H_
#define __BASE_LIB_TYPEDEF_HELPER_H_

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

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TYPEDEF_HELPER_GB
#define TYPEDEF_HELPER_EX extern
#else
#define TYPEDEF_HELPER_EX
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

#define BYTE_ALIGN(len, align) (((len) + (align) - 1) & ~((align) - 1))

#define ALIGN_INT(len)  BYTE_ALIGN(len, sizeof(uint32_t))

#define ALIGN4(len)     BYTE_ALIGN(len, 4)
#define ALIGN3(len)     BYTE_ALIGN(len, 3)
#define ALIGN2(len)     BYTE_ALIGN(len, 2)

TYPEDEF_HELPER_EX void check_datatype_mem_len(void);

#ifdef __cplusplus
}
#endif

#endif

