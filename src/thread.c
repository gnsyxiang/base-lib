/****************************************************************
 * Filename: thread.c
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 23/09 2017 14:49
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	23/09 2017
 ****************************************************************/
#include <pthread.h>

#define THREAD_GB
#include "thread.h"
#undef THREAD_GB

pthread_t thread_create_detached(thread_cb_t cb, void *arg)
{
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int error = pthread_create(&tid, &attr, cb, arg);

    pthread_attr_destroy(&attr);

    if (error)
        return -1;

    return tid;
}

