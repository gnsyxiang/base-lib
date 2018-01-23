/**
 *
 * Release under GPLv2.
 * 
 * @file    file_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 15:07
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 15:07
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "log_helper.h"

#define FILE_HELPER_GB
#include "file_helper.h"
#undef FILE_HELPER_GB

static int _set_fcntl(int fd, long arg)
{
	int flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
		flags = 0;

	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int file_set_nonblocking(int fd)
{
	return _set_fcntl(fd, O_NONBLOCK);
}

/*
 * 作用: 当fork子进程后，仍然可以使用fd。
 *       但执行exec后系统就会自动关闭进程中的fd
 */
int file_close_on_exec(int fd)
{
	return _set_fcntl(fd, FD_CLOEXEC);
}

ssize_t file_write(int fd, const void *buf, size_t cnt)
{
	size_t ret;
	size_t nleft;
	const char *ptr;

	ptr = buf;
	nleft = cnt;

	while (nleft > 0) {
		if ((ret = write(fd, ptr, nleft)) <= 0) {
			if (ret < 0 && errno == EINTR)
				ret = 0;
			else
				return -1;
		}

		nleft -= ret;
		ptr   += ret;
	}

	return cnt;
}

ssize_t file_read(int fd, void *buf, size_t cnt)
{
	size_t ret;
	size_t nleft;
	char *ptr;

	ptr   = buf;
	nleft = cnt;

	while (nleft > 0) {
		if ((ret = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				ret = 0;
			else
				return -1;
		} else if (ret == 0)
			break;

		nleft -= ret;
		ptr   += ret;
	}

	return cnt;
}

