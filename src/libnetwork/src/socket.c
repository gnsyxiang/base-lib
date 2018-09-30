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
#include <netdb.h>

#include <utils/log_helper.h>

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

	memset(&sk->sock_addr, '\0', sizeof(struct sockaddr_in));
	sk->sock_len = sizeof(struct sockaddr_in);

	sk->sock_addr.sin_family        = AF_INET;
	sk->sock_addr.sin_addr.s_addr	= inet_addr(sk->ip);
	sk->sock_addr.sin_port          = htons(sk->port);

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

void socket_connect(socket_t *sk, socket_cb_t socket_cb, int timeout_s, void *data)
{
	struct sockaddr_in addr_src, addr_dst;
	int status;
	int time_cnt = 0;

    int addr_len = sizeof(struct sockaddr_in);

    memset(&addr_src, '\0', addr_len);

	addr_src.sin_family         = AF_INET;
	addr_src.sin_port           = htons(0);
	addr_src.sin_addr.s_addr    = htonl(INADDR_ANY);

    status = bind(sk->fd, (const struct sockaddr *)&addr_src, addr_len);//bind
	if (-1 == status) {
		log_e("bind error!");
		close(sk->fd);
		return ;
	}

	memset(&addr_dst, '\0', addr_len);

	addr_dst.sin_family			= AF_INET;
	addr_dst.sin_port			= htons(sk->port);
	addr_dst.sin_addr.s_addr	= inet_addr(sk->ip);

	do {
		status = connect(sk->fd, (const struct sockaddr *)&addr_dst, addr_len);
		if (status < 0) {
			sleep(1);
		}
	} while ((status < 0) && (time_cnt++ < timeout_s));

	if (status < 0) {
		log_i("connect to server error");
	} else {
		log_i("connect to server");

		socket_cb(data);
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

void socket_udp_set_sockaddr_in(socket_t *sk, int port, char *ip)
{
	sk->sock_addr.sin_family	= AF_INET;
	sk->sock_addr.sin_port		= htons(port);
	if(inet_pton(AF_INET , ip, &sk->sock_addr.sin_addr) <= 0) {
		log_e("inet_pton error");
	}
}

int socket_udp_send_msg(socket_t *sk, const void *msg, int len)
{
	int ret = sendto(sk->fd, msg, len, 0, 
			(struct sockaddr *)&sk->sock_addr, sk->sock_len);
	if(ret < 0) {
		log_e("sendto error");
	}

	return ret;
}

int socket_udp_recv_msg(socket_t *sk, void *msg, int len)
{
	int ret = recvfrom(sk->fd, msg, len, 0, 
			(struct sockaddr *)&sk->sock_addr, &sk->sock_len);
	if(ret < 0) {
		log_e("recvfrom error");
	}

	return ret;
}

int hostname_to_ip(const char *hostname, char * const ip)
{
    struct hostent *hptr = gethostbyname(hostname);
    if (!hptr) {
        log_e("gethostbyname error");
        return -1;
    }

    log_i("official hostname: %s",hptr->h_name);

    char **pptr;
    for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
        log_i("\talias hostname: %s", *pptr);

    switch (hptr->h_addrtype) {
        case AF_INET: {
            int save_ip_flag = 0;
            char str[INET_ADDRSTRLEN] = {0};

            pptr = hptr->h_addr_list;

            for(; *pptr != NULL; pptr++) {
                inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str));
                log_i("\taddr: %s", str);

                if (!save_ip_flag) {
                    save_ip_flag = 1;
                    strncpy(ip, str, INET_ADDRSTRLEN);
                }
            }
            break;
        }
        default:
            log_e("unknown address type");
            break;
    }

    return 0;
}

