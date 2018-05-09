/**
 * 
 * Release under GPLv2.
 * 
 * @file    input-key.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/03 2018 19:53
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/03 2018      create the file
 * 
 *     last modified: 29/03 2018 19:53
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>

#include "dir_helper.h"
#include "log_helper.h"

#define INPUT_KEY_GB
#include "input-key.h"
#undef INPUT_KEY_GB

static void __attribute__((unused)) dump_input_event(struct input_event key_event)
{
	printf("----type: %d \n", key_event.type);
	printf("----code: %d \n", key_event.code);
	printf("----value: %d \n", key_event.value);
}

void key_handle_cb(const char *base_path, const char *name, unsigned char d_type, void *args)
{
	/*log_i("base_path: %s, name: %s \n", base_path, name);*/

	char dev_name[INPUT_DEV_NAME_LEN] = {0};
	unsigned char mask[EV_MAX / 8 + 1];
	input_fd_t *input_fd = args;

	sprintf(dev_name, "%s/%s", base_path, name);

	int fd = open(dev_name, O_RDONLY);
	if (fd < 0)
		log_e("failed to open %s, %s", dev_name, strerror(errno));

	ioctl(fd, EVIOCGBIT(0, sizeof(mask)), mask);
	if (mask[EV_KEY / 8] & (1 << (EV_KEY % 8))) {
		ioctl(fd, EVIOCGNAME(sizeof(name)), name);

		input_fd->name[input_fd->len] = calloc(1, sizeof(name) + 1);
		if (input_fd->name[input_fd->len])
			strcpy(input_fd->name[input_fd->len], name);

		input_fd->fd[input_fd->len++]=fd; 
	}
}

static int filter(const struct dirent *dir)
{
	return strncmp("event", dir->d_name, 5) == 0;
}

input_fd_t key_open(void)
{
	input_fd_t input_fd;
	memset(&input_fd, '\0', sizeof(input_fd));

	scan_dir_sort_file(INPUT_DEV_PATH, filter, key_handle_cb, &input_fd);

	return input_fd;
}

void key_close(input_fd_t *input_fd)
{
	for (int i = 0; i < input_fd->len; i++) {
		close(input_fd->fd[i]);
		input_fd->fd[i] = -1;

		if (input_fd->name[i]) {
			free(input_fd->name[i]);
			input_fd->name[i] = NULL;
		}
	}
}

void dump_key_event(key_event_t key_event)
{
	printf("----------key_type: %d \n", key_event.key_type);
	printf("----------is_press: %d \n", key_event.is_press);
}


int read_key_event(int fd, key_event_t *key_event)
{
	struct input_event event;

	int ret = read(fd, &event, sizeof(event));
	if (ret < 0) {
		log_e("%s: read key_event error, %s(%d)\n", __func__, strerror(errno), -errno);
		return ret;
	}

	dump_input_event(event);

	if ((event.type == EV_KEY) && (event.code < BTN_MISC)) {
		key_event->key_type = event.code;
		key_event->is_press = event.value;

		return 1;
	}

	return 0;
}

int _read_keys_event(int *fds, int len, key_event_t *key_event, int timeout_ms)
{
	int fd_max = -1;
	int ret;
	struct timeval tv;

	fd_set read_fs;
	FD_ZERO(&read_fs);

	for (int i = 0; i < len; i++) {
		fd_max = fd_max > fds[i] ? fd_max : fds[i];
		FD_SET(fds[i], &read_fs);
	}

	if (timeout_ms > 0) {
		tv.tv_sec = timeout_ms / 1000;
		tv.tv_usec = (timeout_ms % 1000) * 1000;

		ret = select(fd_max + 1, &read_fs, NULL, NULL, &tv);
	} else {
		ret = select(fd_max + 1, &read_fs, NULL, NULL, NULL);
	}
	if (ret < 0) {
		printf("%s: select error: %s(%d)\n", __func__, strerror(errno), -errno);
		return 0;
	}

	for (int i = 0; i < len; i++)
		if (FD_ISSET(fds[i], &read_fs))
			return read_key_event(fds[i], key_event);

	return 0;
}

