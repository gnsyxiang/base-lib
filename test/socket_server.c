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
	char buffer[BUFFER_SIZE];
	socket_t *client_sk = (socket_t *)args;

	while(1)
	{
		memset(buffer,0,sizeof(buffer));

		socket_read(client_sk, buffer, sizeof(buffer));
		printf("buf: %s \n", buffer);

		if(strcmp(buffer,"exit")==0)
			break;
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
