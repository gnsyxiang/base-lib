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
#include "pthread_helper.h"
#include "network_protocol.h"
#include "network_protocol_client.h"
#include "parse_cmd.h"

static socket_t *client_sk;

static void handle_server_message_cb(unsigned char *message, int len)
{
	print_hex(message, len);
	printf("cmd_type: %d \n\n", message[4]);
}

static void *client_send_message_thread(void *args)
{
	unsigned char buf[] = {0xaa, 0x06, 0x00, 0x01, 0x01, 0xbb, 0x55};

	while (!get_client_read_running_flag())
		usleep(100);

	while (get_client_read_running_flag()) {
		usleep(1 * 1000 * 1000);

		client_send_message(client_sk, buf, sizeof(buf));
	}
	
	return NULL;
}

static int socket_client(void)
{
	int client_read_timeout_ms = 1000;

	create_a_attached_thread(NULL, client_send_message_thread, NULL);

	client_sk = network_protocol_client_init(handle_server_message_cb, client_read_timeout_ms);

	while (1) {
		usleep(1 * 1000 * 1000);
	}

	socket_clean_client(client_sk);

    return 0;
}

void socket_client_test_init(void)
{
	handle_test_cmd_t socket_client_test_cmd[] = {
		{"1", socket_client},
	};

	register_test_cmd(socket_client_test_cmd, ARRAY_NUM(socket_client_test_cmd));
}

