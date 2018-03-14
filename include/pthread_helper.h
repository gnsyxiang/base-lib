/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    pthread_helper.h
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
#ifndef _PTHREAD_HELPER_H_
#define _PTHREAD_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#ifndef PTHREAD_HELPER_GB
#define PTHREAD_HELPER_EX extern
#else
#define PTHREAD_HELPER_EX
#endif

typedef void *(*start_routine_t)(void *arg);

PTHREAD_HELPER_EX void create_a_attached_thread(pthread_t *thread, start_routine_t start_routine, void *arg);
PTHREAD_HELPER_EX void create_a_lower_attached_thread(pthread_t *thread, start_routine_t start_routine, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* end _PTHREAD_HELPER_H_ */

