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
#include "network_protocol.h"

void handle_server_message_cb(unsigned char *message, int len)
{
	char cmd_type = message[4];

	switch (cmd_type) {
		case 1:
			printf("recv ---1\n");
			break;
		case 3:
			printf("recv ---3\n");
			break;
		default:
			break;
	}

	print_hex(message, len);
}

int socket_client(void)
{
	int client_read_timeout_ms = 1000;

	network_protocol_client_init(handle_server_message_cb, client_read_timeout_ms);

	while (1) {
		usleep(1 * 1000 * 1000);
	}

    return 0;
}
