/**
 *
 * Release under GPLv2.
 * 
 * @file    mem_helper.h
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
#ifndef _BASE_LIB_MEM_HELPER_H_
#define _BASE_LIB_MEM_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_MEM_HELPER_GB
#define BASE_LIB_MEM_HELPER_EX extern
#else
#define BASE_LIB_MEM_HELPER_EX
#endif

#define REALLOC_SIZE (32)

void *malloc_mem(size_t len);
void *alloc_mem(int len);
int _realloc_mem(void **pp, int len, int size);
void _free_mem(void **pp);

#define realloc_mem(pp, len, size) _realloc_mem((void **)&(pp), len, size)
#define free_mem(p) _free_mem((void **)&(p))

#ifdef __cplusplus
}
#endif

#endif

