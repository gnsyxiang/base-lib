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
#include "hex_helper.h"

int socket_client(void)
{
	int cnt = 0;
	unsigned char buf[BUF_LEN] = { \
		0x11, 0xaa, 0x07, 0x0, 0x1, 0x1, 0x7, 0xbb, 0x55, \
		0x11, 0xaa, 0x07, 0x0, 0x1, 0x2, 0x7, 0xbb, 0x55, \
		0x11, 0xaa, 0x07, 0x0, 0x1, 0x3, 0x7, 0xbb, 0x55 \
	};

	socket_t *sk_client = socket_init_client("127.0.0.1", MYPORT);
	socket_connect(sk_client, 3);

    while (cnt < 3) {
		socket_write(sk_client, (char *)(buf + cnt * 9), 9);
		print_hex(buf + cnt * 9, 9);
		cnt++;
    }

	sleep(7);

	socket_clean_client(sk_client);

    return 0;
}
