/**
 *
 * Release under GPLv2.
 * 
 * @file    pthread_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    13/12 2017 16:19
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        13/12 2017      create the file
 * 
 *     last modified: 13/12 2017 16:19
 */
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <assert.h>

#define PTHREAD_HELPER_GB
#include "pthread_helper.h"
#undef PTHREAD_HELPER_GB

void create_a_attached_thread(pthread_t *thread, start_routine_t start_routine, void *arg)
{
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_t t;
	if (thread == NULL)
		thread = &t;

	int error = pthread_create(thread, &attr, start_routine, arg);

    pthread_attr_destroy(&attr);

	assert(!error);
}

void create_a_lower_attached_thread(pthread_t *thread, start_routine_t start_routine, void *arg)
{
	int policy;
	struct sched_param param;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	//设置继承的调度策略
	//必需设置inher的属性为 PTHREAD_EXPLICIT_SCHED，否则设置线程的优先级会被忽略
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

	pthread_attr_getschedpolicy(&attr, &policy);
	param.sched_priority = sched_get_priority_min(policy);
	pthread_attr_setschedparam(&attr, &param);

	pthread_t t;
	if (thread == NULL)
		thread = &t;

	int error = pthread_create(thread, &attr, start_routine, arg);

    pthread_attr_destroy(&attr);

	assert(!error);
}

