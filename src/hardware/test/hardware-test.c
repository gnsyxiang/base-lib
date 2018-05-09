/**
 * 
 * Release under GPLv2.
 * 
 * @file    hardware.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/03 2018 18:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/03 2018      create the file
 * 
 *     last modified: 30/03 2018 18:13
 */
#include <stdio.h>

#include "input-key.h"

int main(int argc, char **argv)
{
	input_fd_t input_fd = key_open();

	/*int fd = input_fd.fd[0];*/
	key_event_t key_event;
	int cnt = 0;
	int ret;

	while (1) {
		/*ret = read_key_event(fd, &key_event);*/
		ret = read_keys_event(input_fd.fd, input_fd.len, &key_event);
		if (ret != 1)
			continue;

		if (cnt++ > 8)
			break;

		dump_key_event(key_event);
	}

	key_close(&input_fd);

	return 0;
}
