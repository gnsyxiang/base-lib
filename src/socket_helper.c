/**
 *
 * Release under GPLv2.
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

#include "pthread_helper.h"
#include "log_helper.h"
#include "file_helper.h"

#define SOCKET_HELPER_GB
#include "socket_helper.h"
#undef SOCKET_HELPER_GB

static socket_t *_socket_init_struct(int fd, char *ipaddr, int port)
{
	socket_t *sk = (socket_t *)malloc(sizeof(socket_t));
	if (NULL == sk) {
		log_e("malloc faild");
		exit(-1);
	}

	sk->fd = fd;
	sk->port = port;

	pthread_mutex_init(&sk->mutex, NULL);

	if (ipaddr) {
		int ipaddr_len = strlen(ipaddr) + 1;

		sk->ipaddr = (char *)malloc(ipaddr_len);
		if (NULL == sk->ipaddr) {
			log_e("malloc faild");
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

socket_t *socket_client_init(char *ipaddr, int port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		log_e("socket faild to create client");
		exit(-1);
	}

	return _socket_init_struct(fd, ipaddr, port);
}

void socket_client_clean(socket_t *sk)
{
	if (NULL == sk)
		return;

	close(sk->fd);

	_socket_clean_struct(sk);
}

socket_t *socket_server_init(int port)
{
	socket_t *sk;
	int ret;

	sk = socket_client_init(NULL, port);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int val = 1;
	ret = setsockopt(sk->fd, SOL_SOCKET, SO_REUSEADDR, (void *)&val, sizeof(int));

    ret = bind(sk->fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		log_e("bind faild");
		exit(-1);
	}

	return sk;
}

void socket_server_clean(socket_t *sk)
{
	return socket_client_clean(sk);
}

int socket_set_nonblocking(socket_t *sk)
{
	return file_set_nonblocking(sk->fd);
}

void socket_set_recv_timeout(socket_t *sk, int timeout_ms)
{
	struct timeval timeout;

	timeout.tv_sec = timeout_ms / 1000;
	timeout.tv_usec = (timeout_ms % 1000) * 1000;

	setsockopt(sk->fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
}

void socket_connect(socket_t *sk, socket_cb_t socket_cb, int timeout)
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
	} else {
		printf("connect to server \n");

		socket_cb(sk);
	}
}

socket_t *socket_wait_for_connect(socket_t *sk, socket_cb_t socket_cb)
{
	int fd = sk->fd;

	int status = listen(fd, SOMAXCONN);
	if (status < 0) {
		log_e("listen faild");
		return NULL;
	}

	while (1) {
		fd_set read_fs;
		FD_ZERO(&read_fs);
		FD_SET(fd, &read_fs);
		
		int ret = select(FD_SETSIZE, &read_fs, NULL, NULL, NULL);
		if (ret < 0) {
			log_e("select faild");
			break;
		}

		if (FD_ISSET(fd, &read_fs)) {
			int client_fd = accept(fd, NULL, NULL);
			if (client_fd < 0) {
				log_e("accept faild");
				return NULL;
			}

			socket_cb((void *)&client_fd);
		}
	}
	return NULL;
}


int socket_write(socket_t *sk, const char *buf, int size)
{
	return file_write(sk->fd, buf, size);
}

int socket_read(socket_t *sk, char *buf, int size)
{
	return file_read(sk->fd, buf, size);
}






