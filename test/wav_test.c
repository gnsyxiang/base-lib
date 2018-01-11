/**
 *
 * Release under GPLv2.
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
#include "mem_helper.h"

#define CHANNELS		(1)
#define SAMPLE_RATE		(16000)
#define BIT_PER_SAMPLE	(16)

#define WAV_MS_LEN		(5)

#define NEW_WAV_PATH "new_wav"

static int wav_test(void)
{
	wav_file_param_t wav_file_param;
	wav_file_t *wav_file;
	
	strcpy(wav_file_param.path, "test.wav");
	wav_file_param.channels = CHANNELS;
	wav_file_param.sample_rate = SAMPLE_RATE;
	wav_file_param.bit_per_sample = BIT_PER_SAMPLE;

	wav_file = wav_file_create(&wav_file_param);

	char a = 0x55;
	for (int i = 0; i < 5000; i++)
		wav_file_write(wav_file, &a, 1);
		
	a = 0xc5;
	for (int i = 0; i < 5000; i++)
		wav_file_write(wav_file, &a, 1);

	wav_file_flush(wav_file);
	wav_header_dump(wav_file);
	wav_file_clean(wav_file);

	log_i("wav test OK");

	return 0;
}


int read_wav_to_buf(char *wav_path, char **voice)
{
	wav_file_t *wav_file = wav_file_open(wav_path);

	*voice = malloc_mem(wav_file->wav_header->data_sz);

	int len = wav_file_read(wav_file, *voice, wav_file->wav_header->data_sz);

	wav_file_clean(wav_file);

	return len;
}

void write_buf_to_wav(wav_file_param_t *wav_file_param, char *voice, int len, int wav_ms)
{
	wav_file_t *new_wav_file;

	new_wav_file = wav_file_create(wav_file_param);

    int total_bytes = wav_ms * SAMPLE_RATE * 2;
    int blank_bytes = (total_bytes - len) / 2;

	switch (wav_file_param->bit_per_sample / 8) {
		case 2: blank_bytes = ALIGN2(blank_bytes); break;
		case 3: blank_bytes = ALIGN3(blank_bytes); break;
		case 4: blank_bytes = ALIGN4(blank_bytes); break;

		default: log_i("bsp is error"); break;
	}

    char buf[blank_bytes];
	memset(buf, '\0', blank_bytes);

	wav_file_write(new_wav_file, buf, blank_bytes);
	wav_file_write(new_wav_file, voice, len);
	wav_file_write(new_wav_file, buf, blank_bytes);

	wav_file_clean(new_wav_file);
}

void wav_handle(const char *base_path, const char *name)
{
	char *voice;
    char src_name[256] = {0};
    char dst_name[256] = {0};
    char dst_dir[256] = {0};
	wav_file_param_t wav_file_param = {0};

    sprintf(src_name, "%s/%s", base_path, name);
    sprintf(dst_dir, "%s/%s", base_path, NEW_WAV_PATH);
    sprintf(dst_name, "%s/%s", dst_dir, name);

    if (access(dst_dir, F_OK) != 0) {
        mkdir(dst_dir, S_IRWXU);
    }

	strcpy(wav_file_param.path, dst_name);

	wav_file_param.channels = CHANNELS;
	wav_file_param.sample_rate = SAMPLE_RATE;
	wav_file_param.bit_per_sample = BIT_PER_SAMPLE;

	int len = read_wav_to_buf(src_name, &voice);
	write_buf_to_wav(&wav_file_param, voice, len, WAV_MS_LEN);

	free_mem(voice);
}

static int add_blank_time_to_wav(void)
{
    char base_path[1000] = {0};

    getcwd(base_path, 999);
	strcat(base_path, "/wav");

    read_file_list(base_path, wav_handle);

	log_i("add blank time to wav OK");

    return 0;
}

void wav_test_init(void)
{
	handle_test_cmd_t wav_test_cmd[] = {
		{"5", wav_test},
		{"6", add_blank_time_to_wav},
	};

	register_test_cmd(wav_test_cmd, ARRAY_NUM(wav_test_cmd));
}

