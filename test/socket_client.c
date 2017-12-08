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
	int cnt = 0;
	unsigned char buf[BUF_LEN] = {0x11, 0xaa, 0x07, 0x0, 0x1, 0x1, 0x7, 0xbb, 0x55};

	socket_t *sk_client = socket_init_client("127.0.0.1", MYPORT);
	socket_connect(sk_client, 3);

    while (cnt++ < 5) {
		socket_write(sk_client, (char *)buf, 9);
    }

	socket_clean_client(sk_client);

    return 0;
}
