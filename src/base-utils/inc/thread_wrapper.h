/**
 *
 * Release under GPLv2.
 * 
 * @file    thread_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/08 2019 20:10
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/08 2019      create the file
 * 
 *     last modified: 08/08 2019 20:10
 */
#ifndef __BASE_LIB_UTILS_INC_THREAD_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_THREAD_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#ifndef BASE_LIB_UTILS_SRC_THREAD_WRAPPER_GB
#define BASE_LIB_UTILS_SRC_THREAD_WRAPPER_EX extern
#else
#define BASE_LIB_UTILS_SRC_THREAD_WRAPPER_EX
#endif

typedef void *(*thread_loop_t)(void *args);

void create_a_attached_thread(pthread_t *thread_id, thread_loop_t thread_loop, void *args);
void create_a_low_priority_attached_thread(pthread_t *thread_id, thread_loop_t thread_loop, void *args);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_THREAD_WRAPPER_H_ */

