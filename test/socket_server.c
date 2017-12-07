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

void *do_something(void *args)
{    
	socket_t *client_sk = (socket_t *)args;
	unsigned char buf[BUF_LEN], *pbuf;
	int ret;
	int package_len;
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

		print_hex(buf, ret);

		while (*pbuf) {
			sleep(1);
			while (*pbuf != FRONT_SYMBOL)
				pbuf++;

			package_len = pbuf[1] | pbuf[2] << 8;

			if (pbuf[package_len] != TAIL_SYMBOL) {
				pbuf++;
				continue;
			}

			message = (unsigned char *)malloc(package_len + 1);

			strncpy(message, pbuf, package_len + 1);

			handle_message(message, package_len + 1);

			free(message);

			pbuf += package_len + 1;
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

