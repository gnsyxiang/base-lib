/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
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

typedef struct _socket_tag {
	int fd;
	int port;
	char *ipaddr;
	pthread_mutex_t lock;
} socket_t;

#define MYPORT  8887
#define BUF_LEN (1024)

typedef void *(*server_handle_message)(void *args);

SOCKET_HELPER_EX socket_t *socket_init_client(char *ipaddr, int port);
SOCKET_HELPER_EX void socket_clean_client(socket_t *sk);
SOCKET_HELPER_EX socket_t *socket_init_server(int port);
SOCKET_HELPER_EX void socket_clean_server(socket_t *sk);

SOCKET_HELPER_EX int socket_set_nonblocking(socket_t *sk);
SOCKET_HELPER_EX void socket_set_recv_timeout(socket_t *sk, int timeout_ms);


SOCKET_HELPER_EX void socket_connect(socket_t *sk, int timeout);
SOCKET_HELPER_EX int socket_wait_for_connect(socket_t *sk, server_handle_message callback);

SOCKET_HELPER_EX int socket_write(socket_t *sk, const char *buf, int size);
SOCKET_HELPER_EX int socket_read(socket_t *sk, char *buf, int size);

#ifdef __cplusplus
}
#endif

#endif /* end _SOCKET_HELPER_H_ */