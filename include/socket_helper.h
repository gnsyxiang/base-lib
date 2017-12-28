/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    socket_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/11 2017 18:21
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/11 2017      create the file
 * 
 *     last modified: 29/11 2017 18:21
 */
#ifndef _SOCKET_HELPER_H_
#define _SOCKET_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <pthread.h>

#ifndef SOCKET_HELPER_GB
#define SOCKET_HELPER_EX extern
#else
#define SOCKET_HELPER_EX
#endif

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

struct socket {
    int fd;
    const char *name;
    pthread_mutex_t lock;
};

typedef void (*link_connect_callback_t)(
        void *user, void *connection);

/**
 * for socket implement
 */
struct link_ops {
	void *(*create_client)(const char *name);
	void *(*create_server)(const char *name);
	void (*delete)(void *connection);

	int (*connect)(void *connection, int timeout);
	int (*wait_for_connect)(void *connection, void *user, link_connect_callback_t cb);

	int (*read)(void *connection, void *buf, int size);
	int (*write)(void *connection, const void *buf, int size);

	int (*get_fd)(void *connection);
};

struct link_ops *get_link_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* end _SOCKET_HELPER_H_ */
