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

#define QUEUE   20

void do_something(int fd)
{    
	char buffer[BUFFER_SIZE];
	int len;

	while(1)
	{
		memset(buffer,0,sizeof(buffer));

		len = recv(fd, buffer, sizeof(buffer),0);
		printf("buf: %s \n", buffer);

		if(strcmp(buffer,"exit")==0)
			break;

		send(fd, buffer, len, 0);
	}

   close(fd);
}

int socket_server(void)
{

	socket_t *sk_server = socket_init_server(MYPORT);

	socket_wait_for_connect(sk_server, do_something);

    close(sk_server->fd);
    return 0;
}
