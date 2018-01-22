/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    file_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:04
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:04
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "log_helper.h"
#include "parse_cmd.h"
#include "file_helper.h"

#define FILE_CONTENT_LEN (10)

static void file_test(void)
{
	int fd;
	char buf[FILE_CONTENT_LEN];

	fd = open("test.txt", O_RDWR);

	int ret = file_read(fd, buf, FILE_CONTENT_LEN);
	log_i("ret: %d", ret);

	log_i("buf: %s", buf);

	close(fd);
}

static void file_test_init(void)
{
	handle_test_cmd_t file_test_cmd[] = {
		{"9", file_test},
	};

	register_test_cmd(file_test_cmd, ARRAY_NUM(file_test_cmd));
}
DECLARE_INIT(file_test_init);

