/**
 *
 * Release under GPLv2.
 * 
 * @file    macro_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/07 2019 10:30
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/07 2019      create the file
 * 
 *     last modified: 12/07 2019 10:30
 */
#ifndef __BASE_LIB_UTILS_INC_MACRO_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_MACRO_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MACRO_ARGS_LEN  (39)

#define macro_args_seqs() \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
     9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define macro_args_n( _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, \
                     _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
                     _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
                     _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
                     \
                     _N, ...) _N

#define macro_get_args_cnt(...) macro_args_n(__VA_ARGS__)

/**
 *  macro_argc: calculate macro args' count, maxim to 39
 */
#define macro_argc(...) macro_get_args_cnt(0, ##__VA_ARGS__, macro_args_seqs())

/**
 * byte alignment
 */
#ifndef BYTE_ALIGN
#define BYTE_ALIGN(len, align) (((len) + (align) - 1) & ~((align) - 1))
#endif

#ifndef ALIGN_INT
#define ALIGN_INT(len)  BYTE_ALIGN(len, sizeof(uint32_t))
#endif

#ifndef ALIGN4
#define ALIGN4(len)     BYTE_ALIGN(len, 4)
#endif

#ifndef ALIGN3
#define ALIGN3(len)     BYTE_ALIGN(len, 3)
#endif

#ifndef ALIGN2
#define ALIGN2(len)     BYTE_ALIGN(len, 2)
#endif

/**
 *
 */
#define GET_RET_DEMO(ret) ({printf("nihao \n"); ret;})

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_MACRO_WRAPPER_H_ */

