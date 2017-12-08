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
typedef void (*handle_message_t)(unsigned char *message, int len);

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

	free(message);
}

void check_is_package(unsigned char *buf, handle_message_t handle_message)
{
	int package_len;
	int is_package_ok = 1;

	while (*buf) {
		while (*buf != FRONT_SYMBOL)
			buf++;

		package_len = buf[1] | buf[2] << 8;

		if (buf[package_len] != TAIL_SYMBOL) {
			buf++;
			is_package_ok = 0;
		}

		if (is_package_ok) {
			unsigned char *message = (unsigned char *)malloc(package_len + 1);

			memcpy(message, buf, package_len + 1);

			handle_message(message, package_len + 1);

			buf += package_len + 1;
		}
	}
}

void *client_thread_callback(void *args)
{    
	socket_t *client_sk = (socket_t *)args;

	while(1) {
		unsigned char buf[BUF_LEN] = {0};

		if (socket_read(client_sk, (char *)buf, 16) <= 0) {
			printf("client socket close \n");
			break;
		}

		check_is_package(buf, handle_message);
	}

	socket_clean_client(client_sk);

	return NULL;
}

int socket_server(void)
{
	socket_t *sk_server = socket_init_server(MYPORT);
	socket_wait_for_connect(sk_server, client_thread_callback);
    close(sk_server->fd);

    return 0;
}

