/**
 * 
 * Release under GPLv2.
 * 
 * @file    thread_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/08 2019 20:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/08 2019      create the file
 * 
 *     last modified: 08/08 2019 20:16
 */
#include <stdio.h>
#include <sched.h>
#include <assert.h>

#define BASE_LIB_UTILS_SRC_THREAD_WRAPPER_GB
#include "thread_wrapper.h"
#undef BASE_LIB_UTILS_SRC_THREAD_WRAPPER_GB

void create_a_attached_thread(pthread_t *thread_id,
                              thread_loop_t thread_loop,
                              void *args)
{
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_t t;
	if (thread_id == NULL)
		thread_id = &t;

	int error = pthread_create(thread_id, &attr, thread_loop, args);

    pthread_attr_destroy(&attr);

	assert(!error);
}

void create_a_low_priority_attached_thread(pthread_t *thread_id,
                                           thread_loop_t thread_loop,
                                           void *args)
{
	int policy;
	struct sched_param param;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	//设置继承的调度策略
	//必需设置inher的属性为PTHREAD_EXPLICIT_SCHED，否则设置线程的优先级会被忽略
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

	pthread_attr_getschedpolicy(&attr, &policy);
	param.sched_priority = sched_get_priority_min(policy);
	pthread_attr_setschedparam(&attr, &param);

	pthread_t t;
	if (thread_id == NULL)
		thread_id = &t;

	int error = pthread_create(thread_id, &attr, thread_loop, args);

    pthread_attr_destroy(&attr);

	assert(!error);
}

