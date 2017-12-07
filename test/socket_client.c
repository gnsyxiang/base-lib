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


int socket_client(void)
{
	socket_t *sk_client = socket_init_client("127.0.0.1", MYPORT);

	socket_connect(sk_client, 3);

    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];
    while (1)
    {
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));

		printf("input your string: ");
		scanf("%s", sendbuf);

		socket_write(sk_client, sendbuf, strlen(sendbuf));

        if(strcmp(sendbuf,"exit")==0)
            break;
    }

	socket_clean_client(sk_client);

    return 0;
}
