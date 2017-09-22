/****************************************************************
 * Filename: type.h
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 21/09 2017 19:57
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	21/09 2017
 ****************************************************************/
#ifndef _TYPE_H_
#define _TYPE_H_

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

#define ALIGN_INT(len)  BYTE_ALIGN(len, sizeof(int))
#define ALIGN4(len)     BYTE_ALIGN(len, 4)

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TYPE_GB
#define TYPE_EX extern
#else
#define TYPE_EX
#endif



#ifdef __cplusplus
}
#endif

#endif /* end _TYPE_H_ */

