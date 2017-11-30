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

int socket_server(void)
{

	socket_t *sk_server = socket_init_server(MYPORT);
    int server_sockfd = sk_server->fd;

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///客户端套接字
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    ///成功返回非负描述字，出错返回-1
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0)
    {
        perror("connect");
        exit(1);
    }

    while(1)
    {
        memset(buffer,0,sizeof(buffer));

        int len = recv(conn, buffer, sizeof(buffer),0);
        if(strcmp(buffer,"exit")==0)
            break;
		printf("buf: %s \n", buffer);

        send(conn, buffer, len, 0);
    }
    close(conn);
    close(server_sockfd);
    return 0;
}
