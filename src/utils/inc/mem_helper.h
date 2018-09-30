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
#ifndef __UTILS_MEM_HELPER_H_
#define __UTILS_MEM_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#ifndef UTILS_MEM_GB
#define UTILS_MEM_EX extern
#else
#define UTILS_MEM_EX
#endif

#define REALLOC_SIZE (32)

int _realloc_mem_by_size(void **pp, int len, int size);
void *_realloc_mem(void **pp, int size);
void _free_mem(void **pp);

UTILS_MEM_EX void *malloc_mem(size_t len);
UTILS_MEM_EX void *alloc_mem(int len);

#define realloc_mem_by_size(p, len, size) \
            _realloc_mem_by_size((void **)&(p), len, size)

#define realloc_mem(p, size) \
            _realloc_mem((void **)&(p), size)

#define free_mem(p) \
            _free_mem((void **)&(p))

#ifdef __cplusplus
}
#endif

#endif /* __UTILS_MEM_HELPER_H_ */

