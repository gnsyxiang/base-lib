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

	int ipaddr_len = strlen(ipaddr) + 1;
	sk->ipaddr = (char *)malloc(ipaddr_len);
	if (NULL == sk->ipaddr) {
		printf("%s: malloc faild: %s(%d) \n",
				__func__, strerror(errno), -errno);
		exit(-1);
	}

	memset(sk->ipaddr, '\0', ipaddr_len);
	strcpy(sk->ipaddr, ipaddr);

	return sk;
}

static void _socket_clean_struct(socket_t *sk)
{
	if (NULL == sk)
		return;

	if (NULL != sk->ipaddr)
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

void socket_connect(socket_t *sk, int timeout)
{
	struct sockaddr_in addr;
	int status;
	int time;

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

	if (status < 0)
		printf("wait for connect to server error !!! \n");
	else
		printf("connect to server \n");
}

int socket_write(socket_t *sk, const char *buf, int size)
{
	int offset = 0;
	int sz = size;

	while (sz > 0) {
		int bytes = write(sk->fd, buf + offset, sz);
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
		int bytes = read(sk->fd, buf + offset, sz);
		printf("bytes: %d \n", bytes);
		if (bytes <= 0) {
			return bytes;
		}

		sz -= bytes;
		offset += bytes;
	}

	return size;
}






