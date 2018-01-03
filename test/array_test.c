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
#include "array_reader.h"

#define ARRAY_WRITE_LEN (20)

void *array_read_thread(void *args)
{
	char buf[] = {
		0x01, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00,
	};
	struct array_reader *ar = create_array_reader(buf, sizeof(buf));

	log_i("haha");

	array_reader_dump_buffer(ar);

	int a;
	while (!array_reader_is_empty(ar)) {
		a = array_reader_read_int(ar);
		log_i("a: %d", a);
	}

	delete_array_reader(ar);

	return NULL;
}

void *array_write_thread(void *args)
{
	char a = 'a';
	int cnt = 0;
	struct array_writer *aw = create_array_writer(ARRAY_WRITE_LEN);

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

	sleep(2);

	return 0;
}

void array_init(void)
{
	handle_test_cmd_t array_test_cmd[] = {
		{"10", array_test},
	};

	register_test_cmd(array_test_cmd, ARRAY_NUM(array_test_cmd));
}
