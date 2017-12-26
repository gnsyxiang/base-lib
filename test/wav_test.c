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
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "wav_helper.h"
#include "parse_cmd.h"
#include "log_helper.h"
#include "dir_helper.h"
#include "wav_helper.h"
#include "type_helper.h"

int add_blank_time_to_wav(void);

static int wav_test(void)
{
	wav_file_param_t wav_file_param;
	wav_file_t *wav_file;
	
	strcpy(wav_file_param.path, "./wav_test.wav");
	wav_file_param.channels = 2;
	wav_file_param.sample_rate = 16000;
	wav_file_param.bit_per_sample = 16;

	wav_file = wav_file_create(&wav_file_param);

	char a = 0x55;
	for (int i = 0; i < 5000; i++) {
		wav_file_write(wav_file, &a, 1);
	}
		
	a = 0xc5;
	for (int i = 0; i < 5000; i++) {
		wav_file_write(wav_file, &a, 1);
	}

	wav_file_flush(wav_file);

	wav_header_dump(wav_file);

	wav_file_clean(wav_file);

	log_i("wav test");

	add_blank_time_to_wav();

	return 0;
}

#define NEW_WAV_PATH "new_wav"


extern void wav_handle(const char *base_path, const char *name);

int add_blank_time_to_wav(void)
{
    char basePath[1000];

    memset(basePath,'\0',sizeof(basePath));
    getcwd(basePath, 999);
    printf("the current dir is : %s\n",basePath);

    ///get the file list
    memset(basePath,'\0',sizeof(basePath));
    strcpy(basePath,"./wav");

    read_file_list(basePath, wav_handle);

    return 0;
}

void wav_handle(const char *base_path, const char *name)
{
	wav_file_t *wav_file;
	wav_file_t *new_wav_file;
	wav_file_param_t wav_file_param;
	
    char src_name[256] = {0};
    char dst_name[256] = {0};
    char dst_dir[256] = {0};

    sprintf(src_name, "%s/%s", base_path, name);
    sprintf(dst_dir, "%s/%s", base_path, NEW_WAV_PATH);
    sprintf(dst_name, "%s/%s", dst_dir, name);

    if (access(dst_dir, F_OK) != 0) {
        mkdir(dst_dir, S_IRWXU);
    }

	memset(&wav_file_param, '\0', sizeof(wav_file_param));
	strcpy(wav_file_param.path, src_name);

	wav_file = wav_file_open(&wav_file_param);

	memset(&wav_file_param, '\0', sizeof(wav_file_param));
	strcpy(wav_file_param.path, dst_name);
	wav_file_param.channels = 1;
	wav_file_param.sample_rate = 16000;
	wav_file_param.bit_per_sample = 16;

	new_wav_file = wav_file_create(&wav_file_param);

    int total_bytes;
    total_bytes = 15 * 16000 * 2;

    int bps = wav_file->wav_header->fmt_bits_per_sample / 8;
    int blank_bytes;
    blank_bytes = (total_bytes - wav_file->wav_header->data_sz) / 2;
    if (bps == 2)
        blank_bytes = ALIGN2(blank_bytes);
    else if (bps == 3)
        blank_bytes = ALIGN3(blank_bytes);
    else if (bps == 4)
        blank_bytes = ALIGN4(blank_bytes);

    char buf[blank_bytes];
    char voice[wav_file->wav_header->data_sz];

    memset(buf, '\0', blank_bytes);

	wav_file_write(new_wav_file, buf, blank_bytes);

	wav_file_seek(new_wav_file, WAV_HEADER_LEN + blank_bytes, SEEK_SET);
	wav_file_read(wav_file, voice, wav_file->wav_header->data_sz);
	wav_file_write(new_wav_file, voice, wav_file->wav_header->data_sz);

	wav_file_write(new_wav_file, buf, blank_bytes);

	wav_file_clean(new_wav_file);
	wav_file_clean(wav_file);
}

void wav_test_init(void)
{
	handle_test_cmd_t wav_test_cmd[] = {
		{"5", wav_test},
	};

	register_test_cmd(wav_test_cmd, ARRAY_NUM(wav_test_cmd));
}

