/**
 *
 * Release under GPLv2.
 * 
 * @file    mem_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/07 2019 20:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/07 2019      create the file
 * 
 *     last modified: 23/07 2019 20:06
 */
#ifndef __BASE_LIB_UTILS_INC_MEM_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_MEM_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_UTILS_SRC_MEM_WRAPPER_GB
#define BASE_LIB_UTILS_SRC_MEM_WRAPPER_EX extern
#else
#define BASE_LIB_UTILS_SRC_MEM_WRAPPER_EX
#endif

#ifndef CHECK_MEM

#define mem_calloc(num, size) _mem_calloc(__FILE__, __func__, __LINE__, (num), (size))
#define mem_free(p)           _mem_free(__FILE__, __func__, __LINE__, (void **)&(p))

void mem_leak_display(void);

/*
 * functions called by helper macros, don't care those functions
 */
void *_mem_calloc(const char *file, const char *func, size_t line, int num, size_t size);
void _mem_free(const char *file, const char *func, size_t line, void **p);

#else

#define mem_calloc(num, size) calloc((num), (size));
#define mem_free(p)           free((p)); 

#endif

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_MEM_WRAPPER_H_ */

