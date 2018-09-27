/**
 *
 * Release under GPLv2.
 * 
 * @file    socket.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    11/07 2018 14:30
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        11/07 2018      create the file
 * 
 *     last modified: 11/07 2018 14:30
 */
#ifndef __NET_SOCKET_H_
#define __NET_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <netinet/ip.h>

#define TEST_PORT	(8888)
#define TEST_IP		"127.0.0.1"

typedef struct socket_tag {
	int type;

	int port;
	char *ip;

	int fd;
	int loop_flag;

	struct sockaddr_in sock_addr;
	socklen_t sock_len;

	pthread_mutex_t mutex;
}socket_t;

typedef void (*socket_cb_t)(void *argv);

socket_t *socket_udp_client_init(int port, char *ip);
socket_t *socket_tcp_client_init(int port, char *ip);

void socket_udp_client_clean(socket_t *sk);
void socket_tcp_client_clean(socket_t *sk);

socket_t *socket_udp_server_init(int port, char *ip);
socket_t *socket_tcp_server_init(int port, char *ip);

void socket_udp_server_clean(socket_t *sk);
void socket_tcp_server_clean(socket_t *sk);

void socket_connect(socket_t *sk, socket_cb_t socket_cb, int timeout_s, void *data);
void socket_udp_wait_for_connect(socket_t *sk, socket_cb_t socket_cb);
void socket_tcp_wait_for_connect(socket_t *sk, socket_cb_t socket_cb);

void socket_udp_set_sockaddr_in(socket_t *sk, int port, char *ip);

int socket_udp_send_msg(socket_t *sk, const void *msg, int len);
int socket_udp_recv_msg(socket_t *sk, void *msg, int len);

#ifdef __cplusplus
}
#endif

#endif

