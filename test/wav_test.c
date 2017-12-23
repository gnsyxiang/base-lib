/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    wav_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/12 2017 22:22
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/12 2017      create the file
 * 
 *     last modified: 23/12 2017 22:22
 */
#include <stdio.h>
#include <string.h>

#include "wav_helper.h"
#include "parse_cmd.h"
#include "log_helper.h"

static int wav_test(void)
{
	wav_file_param_t wav_file_param;
	wav_file_t *wav_file;
	
	strcpy(wav_file_param.path, "./wav_test.wav");
	wav_file_param.channels = 2;
	wav_file_param.sample_rate = 16000;
	wav_file_param.bit_per_sample = 16;

	wav_file = wav_file_init(&wav_file_param);

	wav_file_clean(wav_file);

	log_i("wav test");

	return 0;
}

void wav_test_init(void)
{
	handle_test_cmd_t wav_test_cmd[] = {
		{"5", wav_test},
	};

	register_test_cmd(wav_test_cmd, ARRAY_NUM(wav_test_cmd));
}

