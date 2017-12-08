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

void handle_message(unsigned char *buf, int len)
{
	unsigned char *message;

	message = (unsigned char *)malloc(len);

	memcpy(message, buf, len);

	print_hex(message, len);

	free(message);
}

void check_is_package(unsigned char *buf, int ret, handle_message_t handle_message)
{
	int cur_status = 1;
	int package_len;
	unsigned char *pbuf;
	static unsigned char buf_tmp[BUF_LEN] = {0};
	static int store_left_bytes = 0;

	printf("-------1--\n");
	memcpy(buf_tmp + store_left_bytes, buf, ret);
	print_hex(buf_tmp, ret + store_left_bytes);

	ret += store_left_bytes;

	pbuf = buf_tmp;

	while ((ret) > 0) {
		switch (cur_status) {
			case 1:
				printf("-----1\n");
				while (*pbuf != FRONT_SYMBOL) {
					pbuf++;
					ret--;
				}
				cur_status++;
				break;
			case 2:
				printf("-----2\n");
				package_len = pbuf[1] | pbuf[2] << 8;

				if (pbuf[package_len] != TAIL_SYMBOL && pbuf[package_len] != 0x0) {
					pbuf++;
					ret--;
					cur_status = 1;
					break;
				}
				cur_status++;
				break;
			case 3:
				printf("-----3\n");
				if (pbuf[package_len] == TAIL_SYMBOL) {
					handle_message(pbuf, package_len + 1);

					pbuf += package_len + 1;
					ret -= package_len + 1;
					cur_status = 1;
					break;
				}
				cur_status++;
				break;
			case 4:
				printf("-----4\n");
				memcpy(buf_tmp, pbuf, ret);
				print_hex(buf_tmp, ret);

				store_left_bytes = ret;
				ret = 0;
				break;
			default:
				printf("error status \n");
				break;
		}
	}
}

void *client_thread_callback(void *args)
{    
	int ret;
	socket_t *client_sk = (socket_t *)args;

	while(1) {
		unsigned char buf[BUF_LEN] = {0};

		ret = socket_read(client_sk, (char *)buf, 16);
		if (ret <= 0) {
			printf("client socket close \n");
			break;
		}

		check_is_package(buf, ret, handle_message);
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

