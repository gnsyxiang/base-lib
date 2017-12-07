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

void *do_something(void *args)
{    
	int ret;
	unsigned char buf[BUF_LEN];
	socket_t *client_sk = (socket_t *)args;

	while(1)
	{
		sleep(1);

		memset(buf, '\0', sizeof(buf));

		ret = socket_read(client_sk, (char *)buf, 8);
		printf("ret: %d \n", ret);
		if (ret <= 0) {
			printf("client socket close \n");
			break;
		}

		for (int i = 0; i < ret; i++) {
			printf("%02x ", buf[i]);
		}
		printf("\n");
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

