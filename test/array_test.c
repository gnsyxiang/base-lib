/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    array_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/01 2018 15:02
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/01 2018      create the file
 * 
 *     last modified: 03/01 2018 15:02
 */
#include <stdio.h>
#include <unistd.h>

#include "parse_cmd.h"
#include "log_helper.h"
#include "pthread_helper.h"
#include "array_writer.h"

#define ARRAY_WRITE_LEN (20)

struct array_reader *ar;

void *array_read_thread(void *args)
{
	return NULL;
}

void *array_write_thread(void *args)
{
	char a = 'a';
	int cnt = 0;
	struct array_writer *aw;

	aw = create_array_writer(ARRAY_WRITE_LEN);

	while (cnt++ < 10) {
		array_writer_write_int(aw, a++);
	}

	array_writer_dump_buffer(aw);

	delete_array_writer(aw);
	
	return NULL;
}

static int array_test(void)
{
	create_a_attached_thread(NULL, array_write_thread, NULL);
	
	sleep(1);

	create_a_attached_thread(NULL, array_read_thread, NULL);

	return 0;
}

void array_init(void)
{
	handle_test_cmd_t array_test_cmd[] = {
		{"10", array_test},
	};

	register_test_cmd(array_test_cmd, ARRAY_NUM(array_test_cmd));
}
