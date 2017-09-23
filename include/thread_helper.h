/****************************************************************
 * Filename: thread_helper.h
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 23/09 2017 14:51
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	23/09 2017
 ****************************************************************/
#ifndef _THREAD_HELPER_H_
#define _THREAD_HELPER_H_

#ifdef __cplusplus
export "C" {
#endif

#include <pthread.h>

#ifndef THREAD_HELPER_GB
#define THREAD_HELPER_EX extern
#else
#define THREAD_HELPER_EX
#endif

typedef void *(*thread_cb_t)(void *);

THREAD_HELPER_EX pthread_t thread_create_detached(thread_cb_t cb, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* end _THREAD_HELPER_H_ */

