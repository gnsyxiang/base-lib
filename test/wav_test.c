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

#include "parse_cmd.h"
#include "log_helper.h"
#include "dir_helper.h"
#include "wav_helper.h"
#include "type_helper.h"
#include "mem_helper.h"
#include "str_helper.h"

#define SRC_CHANNELS (8)
#define FRAME_CNT (1024)

#define CHANNELS		(1)
#define SAMPLE_RATE		(16000)
#define BIT_PER_SAMPLE	(16)

#define WAV_MS_LEN		(15)

#define TOP_DIR			"wav"
#define SRC_DIR			"src"
#define DST_DIR			"dst"

#define SRC_DIR_PATH	TOP_DIR"/"SRC_DIR

#define DIR_PATH_LEN	(256)
#define EXT_NAME_LEN	(10)

static void wav_test(void)
{
    if (access(SRC_DIR_PATH, F_OK) != 0) {
		system("mkdir -p "SRC_DIR_PATH);
    }

	char wav_path[DIR_PATH_LEN] = {0};
	sprintf(wav_path, "%s/%s", SRC_DIR_PATH, "test.wav");

	wav_file_t *wav_file = wav_file_create(wav_path, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

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

void add_blank_time(void *file, void *new_file)
{
	wav_file_t *wav_file = file;
	wav_file_t *new_wav_file = new_file;

	int wav_data_len = wav_file->data->data_sz;
	char *voice = alloc_mem(wav_data_len);
	int len = wav_file_read(wav_file, voice, wav_data_len);

    int total_bytes = WAV_MS_LEN * SAMPLE_RATE * 2;
    int blank_bytes = (total_bytes - len) / 2;

	switch (new_wav_file->fmt->fmt_bits_per_sample / 8) {
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

	free_mem(voice);
}

void save_channel_to_wav(void *file, void *new_file)
{
	wav_file_t *wav_file = file;
	wav_file_t **new_wav_file = new_file;

	static short buf[SRC_CHANNELS * FRAME_CNT];
	static short channels[SRC_CHANNELS][FRAME_CNT];
	int ret;

	while (1) {
		ret = wav_file_read(wav_file, buf, SRC_CHANNELS * FRAME_CNT * sizeof(short));
		if (ret <= 0)
			break;

		for (int j = 0; j < SRC_CHANNELS; j++)
			for (int i = 0; i < FRAME_CNT; i++)
				channels[j][i] = buf[j + SRC_CHANNELS * i];

		for (int j = 0; j < SRC_CHANNELS; j++)
			ret = wav_file_write(new_wav_file[j], channels[j], CHANNELS * FRAME_CNT * sizeof(short));
	}
}

/*#define SAVE_TO_NEW_NAME*/
#define SAVE_WAV_CHANNELS

void save_wav_channels(const char *base_path, const char *name, int d_type)
{
	/*log_i("base_path: %s, name: %s, d_type: %d", base_path, name, d_type);*/

    char src_name[DIR_PATH_LEN] = {0};
    char dst_name[SRC_CHANNELS][DIR_PATH_LEN] = {0};
	wav_file_t *wav_file;
	wav_file_t *new_wav_file[SRC_CHANNELS];
	char new_dst_name[DIR_PATH_LEN] = {0};
	const char *dst_dir = NULL;

	dst_dir = str_replace_substr(base_path, SRC_DIR, DST_DIR, 1);

    sprintf(src_name, "%s/%s", base_path, name);
	for (int i = 0; i < SRC_CHANNELS; i++)
		sprintf(dst_name[i], "%s/ch%d-%s", dst_dir, i, name);

    if (access(dst_dir, F_OK) != 0) {
		sprintf(new_dst_name, "mkdir -p %s", dst_dir);
		system(new_dst_name);
    }

	free_mem(dst_dir);

	log_i("src_name: %s", src_name);

	wav_file = wav_file_open(src_name);
	for (int i = 0; i < SRC_CHANNELS; i++)
		new_wav_file[i] = wav_file_create(dst_name[i], CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

	/*add_blank_time(wav_file, new_wav_file);*/
	save_channel_to_wav(wav_file, new_wav_file);

	wav_file_clean(wav_file);
	for (int i = 0; i < SRC_CHANNELS; i++)
		wav_file_clean(new_wav_file[i]);
}

void wav_handle(const char *base_path, const char *name, int d_type)
{
	save_wav_channels(base_path, name, d_type);
	return;

    char src_name[DIR_PATH_LEN] = {0};
    char dst_name[DIR_PATH_LEN] = {0};
	char new_dst_name[DIR_PATH_LEN] = {0};
	const char *dst_dir = NULL;
#ifdef SAVE_TO_NEW_NAME
	static int file_cnt = 0;
#endif

	dst_dir = str_replace_substr(base_path, SRC_DIR, DST_DIR, 1);

    sprintf(src_name, "%s/%s", base_path, name);
    sprintf(dst_name, "%s/%s", dst_dir, name);

    if (access(dst_dir, F_OK) != 0) {
#ifdef SAVE_TO_NEW_NAME
		file_cnt = 0;
#endif
		sprintf(new_dst_name, "mkdir -p %s", dst_dir);
		system(new_dst_name);
    }

#ifdef SAVE_TO_NEW_NAME
	memset(dst_name, '\0', DIR_PATH_LEN);
    sprintf(dst_name, "%s/%d.wav", dst_dir, ++file_cnt);
#endif
	free_mem(dst_dir);

	log_i("src_name: %s", src_name);

	wav_file_t *wav_file = wav_file_open(src_name);
	wav_file_t *new_wav_file = wav_file_create(dst_name, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

	/*add_blank_time(wav_file, new_wav_file);*/
	save_channel_to_wav(wav_file, new_wav_file);

	wav_file_clean(wav_file);
	wav_file_clean(new_wav_file);
}

#define EXT_NAME	"wav"
int file_filter(const struct dirent *file)
{
	/*log_i("name: %s", file->d_name);*/

	if(file->d_type != DT_REG)
		return 0;

	char ext_name[EXT_NAME_LEN] = {0};
	str_get_file_extension_name(file->d_name, ext_name);

	return (strncmp(ext_name, EXT_NAME, strlen(EXT_NAME)) == 0);
}

void handle_cb(const char *base_path, const char *name, int d_type)
{
	/*log_i("base_path: %s, name: %s", base_path, name);*/

	if (d_type == DT_REG)
		return;

	char dir_name[DIR_PATH_LEN] = {0};
	sprintf(dir_name, "%s/%s", base_path, name);

	scan_dir_sort_file(dir_name, file_filter, wav_handle);
}

static void create_new_wav(void)
{
	read_file_list(SRC_DIR_PATH, handle_cb);

	log_i("succesful ...");
}

static void wav_test_init(void)
{
	handle_test_cmd_t wav_test_cmd[] = {
		{"5", wav_test},
		{"6", create_new_wav},
	};

	register_test_cmd(wav_test_cmd, ARRAY_NUM(wav_test_cmd));
}
DECLARE_INIT(wav_test_init);

