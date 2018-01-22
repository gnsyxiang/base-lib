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
#include <stdlib.h>

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

#define SRC_DIR "wav/src"
#define DST_DIR "wav/dst"

static void wav_test(void)
{
    if (access(SRC_DIR, F_OK) != 0) {
		system("mkdir -p "SRC_DIR);
    }

	char wav_path[256] = {0};
	wav_file_param_t wav_file_param;

	sprintf(wav_path, "%s/%s", SRC_DIR, "test.wav");
	wav_file_param_init(&wav_file_param, wav_path, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

	wav_file_t *wav_file = wav_file_create(&wav_file_param);

	char a = 0x55;
	char b = 0xc5;
	for (int i = 0; i < 5000 * 2; i++) {
		if (i < 5000)
			wav_file_write(wav_file, &a, 1);
		else 
			wav_file_write(wav_file, &b, 1);
	}

	wav_file_clean(wav_file);

	log_i("wav test OK");
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

void add_blank_time(char *src_name, wav_file_param_t *wav_file_param)
{
	char *voice;
	
	int len = read_wav_to_buf(src_name, &voice);
	write_buf_to_wav(wav_file_param, voice, len, WAV_MS_LEN);

	free_mem(voice);
}

void save_one_channel_to_wav(char *src_name, wav_file_param_t *wav_file_param)
{
#define BUF_LEN (1024)
	static short buf[BUF_LEN * 5 * 2];
	static short one_channel[BUF_LEN];
	int ret;

	wav_file_t *wav_file = wav_file_open(src_name);
	wav_file_t *one_channel_wav_file = wav_file_create(wav_file_param);

	while (1) {
		ret = wav_file_read(wav_file, buf, BUF_LEN * 5);
		if (ret <=0)
			break;

		for (int i = 0; i < BUF_LEN; i++) {
			one_channel[i] = buf[5 * i + 1];
		}

		ret = wav_file_write(one_channel_wav_file, one_channel, BUF_LEN);
	}

	wav_file_clean(wav_file);
	wav_file_clean(one_channel_wav_file);
}

void wav_handle(const char *base_path, const char *name)
{
    char src_name[256] = {0};
    char dst_name[256] = {0};

    sprintf(src_name, "%s/%s", base_path, name);
    sprintf(dst_name, "%s/%s", DST_DIR, name);

	log_i("src_name: %s", src_name);

	wav_file_param_t wav_file_param = {0};
	wav_file_param_init(&wav_file_param, dst_name, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

	/*add_blank_time(src_name, &wav_file_param);*/
	save_one_channel_to_wav(src_name, &wav_file_param);
}

static void add_blank_time_to_wav(void)
{
    if (access(DST_DIR, F_OK) != 0) {
		system("mkdir -p "DST_DIR);
    }

    read_file_list(SRC_DIR, wav_handle);

	log_i("succesful ...");
}

static void wav_test_init(void)
{
	handle_test_cmd_t wav_test_cmd[] = {
		{"5", wav_test},
		{"6", add_blank_time_to_wav},
	};

	register_test_cmd(wav_test_cmd, ARRAY_NUM(wav_test_cmd));
}
DECLARE_INIT(wav_test_init);

