/**
 *
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

typedef void (*socket_cb_t)(void *args);

typedef struct _socket_tag {
	int fd;
	int port;
	char *ipaddr;

	pthread_mutex_t mutex;
} socket_t;
#define SOCKET_T_LEN	(sizeof(socket_t))

#define STATUS_NO_ACCEPT	(1)
#define STATUS_ACCEPT		(!STATUS_NO_ACCEPT)

#define MYPORT  8887
#define LO_IP "127.0.0.1"
#define BUF_LEN (1024)

SOCKET_HELPER_EX socket_t *socket_client_init(char *ipaddr, int port);
SOCKET_HELPER_EX void socket_client_clean(socket_t *sk);
SOCKET_HELPER_EX socket_t *socket_server_init(int port);
SOCKET_HELPER_EX void socket_server_clean(socket_t *sk);

SOCKET_HELPER_EX int socket_set_nonblocking(socket_t *sk);
SOCKET_HELPER_EX void socket_set_recv_timeout(socket_t *sk, int timeout_ms);
SOCKET_HELPER_EX void socket_server_set_accept_flag(int flag);

SOCKET_HELPER_EX void socket_connect(socket_t *sk, socket_cb_t socket_cb, int timeout);
SOCKET_HELPER_EX void socket_wait_for_connect(socket_t *sk, socket_cb_t socket_cb);

SOCKET_HELPER_EX int socket_write(socket_t *sk, const char *buf, int size);
SOCKET_HELPER_EX int socket_read(socket_t *sk, char *buf, int size);

#ifdef __cplusplus
}
#endif

#endif /* end _SOCKET_HELPER_H_ */
