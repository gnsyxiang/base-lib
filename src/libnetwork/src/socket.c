/**
 * 
 * Release under GPLv2.
 * 
 * @file    socket.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    11/07 2018 14:29
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        11/07 2018      create the file
 * 
 *     last modified: 11/07 2018 14:29
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <log_helper.h>

#include "socket.h"

static socket_t *init_socket_tag(int fd, int port, char *ip, int type)
{
	socket_t *sk = (socket_t *)calloc(1, sizeof(socket_t));
	if (!sk) {
		log_e("calloc mem error");
		return NULL;
	}

	sk->fd			= fd;
	sk->loop_flag	= 0;

	sk->port		= port;
	sk->type 		= type;

	pthread_mutex_init(&sk->mutex, NULL);

	if (ip) {
		sk->ip = (char *)calloc(1, strlen(ip) + 1);
		if (NULL == sk->ip) {
			log_e("malloc faild");
			return NULL;
		}

		strcpy(sk->ip, ip);
	}

	return sk;
}

static socket_t *init_socket_client(int port, char *ip, int type)
{
	int fd = socket(AF_INET, type, 0);
	if (fd < 0) {
		log_e("socket faild to create client");
		return NULL;
	}

	return init_socket_tag(fd, port, ip, type);
}

static void clean_socket_tag(socket_t *sk)
{
	if (!sk)
		return;

	if (sk->ip) {
		free(sk->ip);
		sk->ip = NULL;
	}

	pthread_mutex_destroy(&sk->mutex);

	free(sk);
	sk = NULL;
}

static void clean_socket_client(socket_t *sk)
{
	return clean_socket_tag(sk);
}

socket_t *socket_udp_client_init(int port, char *ip)
{
	return init_socket_client(port, ip, SOCK_DGRAM);
}

socket_t *socket_tcp_client_init(int port, char *ip)
{
	return init_socket_client(port, ip, SOCK_STREAM);
}

void socket_udp_client_clean(socket_t *sk)
{
	return clean_socket_client(sk);
}

void socket_tcp_client_clean(socket_t *sk)
{
	return clean_socket_client(sk);
}

static socket_t *init_socket_server(int port, char *ip, int type)
{
	socket_t *sk = init_socket_client(port, ip, type);
	if (!sk) {
		log_e("init_socket_client faild");
		return NULL;
	}

	struct sockaddr_in addr;

	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(port);
	addr.sin_addr.s_addr	= htonl(INADDR_ANY);

	int val = 1;
	int ret = setsockopt(sk->fd, SOL_SOCKET, SO_REUSEADDR, (void *)&val, sizeof(int));

	ret = bind(sk->fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		log_e("bind faild");
		return NULL;
	}

	sk->loop_flag = 1;

	return sk;
}

socket_t *socket_udp_server_init(int port, char *ip)
{
	return init_socket_server(port, ip, SOCK_DGRAM);
}

socket_t *socket_tcp_server_init(int port, char *ip)
{
	return init_socket_server(port, ip, SOCK_STREAM);
}

void socket_udp_server_clean(socket_t *sk)
{
	return clean_socket_client(sk);
}
void socket_tcp_server_clean(socket_t *sk)
{
	return clean_socket_client(sk);
}

void socket_connect(socket_t *sk, socket_cb_t socket_cb, int timeout_s)
{
	struct sockaddr_in addr;
	int status;
	int time_cnt = 0;

	memset(&addr, '\0', sizeof(addr));

	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(sk->port);
	addr.sin_addr.s_addr	= inet_addr(sk->ip);

	do {
		status = connect(sk->fd, (const struct sockaddr *)&addr, sizeof(addr));
		if (status < 0) {
			sleep(1);
		}
	} while ((status < 0) && (time_cnt++ < timeout_s));

	if (status < 0) {
		log_i("connect to server error");
	} else {
		log_i("connect to server");

		socket_cb(sk);
	}
}

void socket_set_server_loop_flag(socket_t *sk, int loop_flag)
{
	sk->loop_flag = loop_flag;
}

int socket_get_server_loop_flag(socket_t *sk)
{
	return sk->loop_flag;
}

void socket_udp_wait_for_connect(socket_t *sk, socket_cb_t socket_cb)
{
	socket_cb((void *)sk);
}

void socket_tcp_wait_for_connect(socket_t *sk, socket_cb_t socket_cb)
{
	int status = listen(sk->fd, SOMAXCONN);
	if (status < 0) {
		log_e("listen faild");
		return;
	}

	while (sk->loop_flag) {
		fd_set read_fs;

		FD_ZERO(&read_fs);
		FD_SET(sk->fd, &read_fs);

		int ret = select(FD_SETSIZE, &read_fs, NULL, NULL, NULL);
		if (ret < 0) {
			log_e("select faild");
			break;
		}

		if (FD_ISSET(sk->fd, &read_fs)) {
			int client_fd = accept(sk->fd, NULL, NULL);
			if (client_fd < 0) {
				log_e("accept faild");
				return;
			}

			socket_t *sk_client = init_socket_tag(client_fd, sk->port, NULL, sk->type);

			socket_cb((void *)sk_client);
		}
	}
	log_i("over accept while");
}

int socket_udp_send_msg(socket_t *sk, const char *msg, int len,
		struct sockaddr_in *sock_addr)
{
	int ret = sendto(sk->fd, msg, len, 0, 
			(struct sockaddr *)sock_addr, sizeof(struct sockaddr_in));
	if(ret < 0) {
		log_e("sendto error");
	}

	return ret;
}

int socket_udp_recv_msg(socket_t *sk, char *msg, int len, 
		struct sockaddr_in *sock_addr)
{
	socklen_t sock_len;

	int ret = recvfrom(sk->fd, msg, len, 0, 
			(struct sockaddr *)sock_addr, &sock_len);
	if(ret < 0) {
		log_e("recvfrom error");
	}

	return ret;
}

