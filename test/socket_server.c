#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#include "socket_helper.h"

#define FRONT_SYMBOL (0xaa)
#define TAIL_SYMBOL (0x55)

void print_hex(unsigned char *buf, int len)
{
	for (int i = 0; i < len; i++) {
		printf("%02x ", buf[i]);
	}
	printf("\n");
}

void handle_message(unsigned char *message, int len)
{
	print_hex(message, len);
}

int check_is_package(unsigned char *buf, unsigned char *message)
{
	int package_len;
	int buf_cnt = 0;
	int is_package_ok = 1;

	while (*buf != FRONT_SYMBOL) {
		buf++;
		buf_cnt++;
	}

	package_len = buf[1] | buf[2] << 8;

	if (buf[package_len] != TAIL_SYMBOL) {
		buf++;
		buf_cnt++;
		is_package_ok = 0;
	}
	print_hex(buf, package_len + 1);

	if (is_package_ok)
		buf_cnt += package_len;

	return buf_cnt;
}

void *do_something(void *args)
{    
	socket_t *client_sk = (socket_t *)args;
	unsigned char buf[BUF_LEN], *pbuf;
	int buf_cnt;
	int ret;
	unsigned char *message;

	while(1) {
		sleep(1);

		memset(buf, '\0', sizeof(buf));

		pbuf = buf;
		ret = socket_read(client_sk, (char *)buf, 9);
		if (ret <= 0) {
			printf("client socket close \n");
			break;
		}

		while (*pbuf) {
			sleep(1);

			buf_cnt = check_is_package(pbuf, message);
			pbuf += buf_cnt + 1;

			/*message = (unsigned char *)malloc(package_len + 1);*/
			/*memcpy(message, pbuf, package_len + 1);*/
			/*handle_message(message, package_len + 1);*/
			/*free(message);*/
		}
	}

	socket_clean_client(client_sk);

	return NULL;
}

int socket_server(void)
{
	socket_t *sk_server = socket_init_server(MYPORT);
	socket_wait_for_connect(sk_server, do_something);
    close(sk_server->fd);

    return 0;
}

