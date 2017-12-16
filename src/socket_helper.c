/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    socket_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/11 2017 18:25
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/11 2017      create the file
 * 
 *     last modified: 29/11 2017 18:25
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>

#include "thread_helper.h"

#define SOCKET_HELPER_GB
#include "socket_helper.h"
#undef SOCKET_HELPER_GB

static socket_t *_socket_init_struct(int fd, char *ipaddr, int port)
{
	socket_t *sk = (socket_t *)malloc(sizeof(socket_t));
	if (NULL == sk) {
		printf("%s: malloc faild: %s(%d) \n",
				__func__, strerror(errno), -errno);
		exit(-1);
	}

	sk->fd = fd;
	sk->port = port;
	pthread_mutex_init(&sk->lock, NULL);

	if (ipaddr) {
		int ipaddr_len = strlen(ipaddr) + 1;
		sk->ipaddr = (char *)malloc(ipaddr_len);
		if (NULL == sk->ipaddr) {
			printf("%s: malloc faild: %s(%d) \n",
					__func__, strerror(errno), -errno);
			exit(-1);
		}

		memset(sk->ipaddr, '\0', ipaddr_len);
		strcpy(sk->ipaddr, ipaddr);
	}

	return sk;
}

static void _socket_clean_struct(socket_t *sk)
{
	if (!sk)
		return;

	if (sk->ipaddr)
		free(sk->ipaddr);

	free(sk);
}

socket_t *socket_init_client(char *ipaddr, int port)
{
	int fd = socket(AF_INET,SOCK_STREAM, 0);
	if (fd < 0) {
		printf("%s: failed to create socket: %s(%d) \n",
				__func__, strerror(errno), -errno);
		exit(-1);
	}

	return _socket_init_struct(fd, ipaddr, port);
}

void socket_clean_client(socket_t *sk)
{
	if (NULL == sk)
		return;

	close(sk->fd);

	_socket_clean_struct(sk);
}

socket_t *socket_init_server(int port)
{
	socket_t *sk;
	int ret;

	sk = socket_init_client(NULL, port);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int val = 1;
	ret = setsockopt(sk->fd, SOL_SOCKET, SO_REUSEADDR, (void *)&val, sizeof(int));

    ret = bind(sk->fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		printf("%s: failed to create socket: %s(%d) \n",
				__func__, strerror(errno), -errno);
		exit(-1);
	}

	return sk;
}

void socket_clean_server(socket_t *sk)
{
	return socket_clean_client(sk);
}

int socket_set_nonblocking(socket_t *sk)
{
	int flags;

	if ((flags = fcntl(sk->fd, F_GETFL, 0)) == -1)
		flags = 0;

	return fcntl(sk->fd, F_SETFL, flags | O_NONBLOCK);
}

void socket_set_recv_timeout(socket_t *sk, int timeout_ms)
{
	struct timeval timeout;

	timeout.tv_sec = timeout_ms / 1000;
	timeout.tv_usec = (timeout_ms % 1000) * 1000;

	setsockopt(sk->fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
}

void socket_connect(socket_t *sk, server_handle_message read_cb, handle_message_t handle_read_message, int timeout)
{
	struct sockaddr_in addr;
	int status;
	int time = 0;

	memset(&addr, '\0', sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(sk->port);
	addr.sin_addr.s_addr = inet_addr(sk->ipaddr);

	do {
		status = connect(sk->fd, (const struct sockaddr *)&addr, sizeof(addr));
		if (status < 0) {
			sleep(1);
		}
	} while ((status < 0) && (time++ < timeout));

	if (status < 0) {
		printf("wait for connect to server error !!! \n");
	}
	else {
		printf("connect to server \n");
		client_read_args_t *client_read_args = (client_read_args_t *)malloc(sizeof(client_read_args_t));
		if (!client_read_args) {
			printf("%s:%d, malloc faild \n", __func__, __LINE__);
			exit(-1);
		}
		client_read_args->client_sk = sk;
		client_read_args->handle_read_message = handle_read_message;

		thread_create_detached(read_cb, (void *)client_read_args);
	}
}

int socket_wait_for_connect(socket_t *sk, server_handle_message callback, handle_message_t handle_message)
{
	int fd = sk->fd;

	int status = listen(fd, SOMAXCONN);
	if (status < 0) {
		printf("%s: failed to listen socket: %s(%d) \n", __func__, strerror(errno), -errno);
		return -1;
	}

	while (1) {
		fd_set read_fs;
		FD_ZERO(&read_fs);
		FD_SET(fd, &read_fs);

		int ret = select(FD_SETSIZE, &read_fs, NULL, NULL, NULL);
		if (ret < 0) {
			printf("%s: select error: %s(%d)\n", __func__, strerror(errno), -errno);
			break;
		}

		if (FD_ISSET(fd, &read_fs)) {
			int client_fd = accept(fd, NULL, NULL);
			if (client_fd < 0) {
				printf("%s: failed to accept socket: %s(%d) \n", __func__, strerror(errno), -errno);
				return -1;
			}

			socket_t *client_sk = _socket_init_struct(client_fd, NULL, sk->port);

			client_read_args_t *client_read_args = (client_read_args_t *)malloc(sizeof(client_read_args_t));
			if (!client_read_args) {
				printf("%s:%d, malloc faild \n", __func__, __LINE__);
				exit(-1);
			}

			client_read_args->client_sk = client_sk;
			client_read_args->handle_read_message = handle_message;

			thread_create_detached(callback, (void *)client_read_args);
		}
	}

	return 0;
}


int socket_write(socket_t *sk, const char *buf, int size)
{
	int offset = 0;
	int sz = size;

	while (sz > 0) {
		int bytes = send(sk->fd, buf + offset, sz, 0);
		if (bytes <= 0) {
			return bytes;
		}

		sz -= bytes;
		offset += bytes;
	}

	return size;
}

int socket_read(socket_t *sk, char *buf, int size)
{
	int offset = 0;
	int sz = size;

	while (sz > 0) {
		int bytes = recv(sk->fd, buf + offset, sz, 0);
		if (bytes == 0) {
			printf("%s: client close fd: %s(%d) \n",
					__func__, strerror(errno), -errno);
			break;
		} else if (bytes == -1) {
			if (offset == 0)
				offset = -1;
			break;
		}

		sz -= bytes;
		offset += bytes;
	}

	return offset;
}






