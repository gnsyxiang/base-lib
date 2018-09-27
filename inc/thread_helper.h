/**
 *
 * Release under GPLv2.
 * 
 * @file    thread_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    13/12 2017 15:38
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        13/12 2017      create the file
 * 
 *     last modified: 13/12 2017 15:38
 */
#ifndef __UTILS_THREAD_HELPER_H_
#define __UTILS_THREAD_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#ifndef UTILS_THREAD_GB
#define UTILS_THREAD_EX extern
#else
#define UTILS_THREAD_EX
#endif

typedef void *(*start_routine_t)(void *arg);

UTILS_THREAD_EX void create_a_attached_thread(pthread_t *thread, 
        start_routine_t start_routine, void *arg);

UTILS_THREAD_EX void create_a_lower_attached_thread(pthread_t *thread, 
        start_routine_t start_routine, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* end __UTILS_THREAD_HELPER_H_ */

