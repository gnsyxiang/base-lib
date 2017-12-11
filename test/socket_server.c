#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hex_helper.h"
#include "network_protocol.h"

void handle_message(unsigned char *buf, int len)
{
	char cmd_type;
	cmd_type = buf[4];

	print_hex(buf, len);

	switch (cmd_type) {
		case 1:
			printf("---------1\n");
			break;
		case 2:
			printf("---------2\n");
			break;
		case 3:
			printf("---------3\n");
			break;
		default:
			break;
	}
}

int socket_server(void)
{
	int read_timeout_ms = 3000;

	network_protocol_server_init(handle_message, read_timeout_ms);

    return 0;
}

