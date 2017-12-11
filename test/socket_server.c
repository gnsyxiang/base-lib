#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hex_helper.h"
#include "network_protocol.h"

void handle_message(unsigned char *buf, int len)
{
	unsigned char *message;

	message = (unsigned char *)malloc(len);

	memcpy(message, buf, len);

	print_hex(message, len);

	free(message);
}

int socket_server(void)
{
	int read_timeout_ms = 3000;

	network_protocol_server_init(handle_message, read_timeout_ms);

    return 0;
}

