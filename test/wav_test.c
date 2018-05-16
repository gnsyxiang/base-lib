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

#define FRAME_CNT		(1024)
#define DIR_PATH_LEN	(128)

#define TOP_DIR			"wav"
#define SRC_DIR			"src"
#define DST_DIR			"dst"

#define SRC_DIR_PATH	TOP_DIR"/"SRC_DIR
#define DST_DIR_PATH	TOP_DIR"/"DST_DIR

static void wav_test(void)
{
#define POINT_NUM	(256000 + 30)

#define CHANNELS		(2)
#define SAMPLE_RATE		(16000)
#define BIT_PER_SAMPLE	(16)

    if (access(SRC_DIR_PATH, F_OK) != 0) {
		system("mkdir -p "SRC_DIR_PATH);
    }

	char wav_path[DIR_PATH_LEN] = {0};
	sprintf(wav_path, "%s/%s", SRC_DIR_PATH, "test.wav");

	wav_file_t *wav_file = wav_file_create(wav_path, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

	short buf[CHANNELS] = {
		0x1800, 0x7800, 
		/*0xd800, 0x8800,*/
		/*0x5800, 0x4800, */
		/*0x3800, 0x2800*/
	};

	for (int i = 0; i < POINT_NUM * CHANNELS; i++)
		wav_file_write(wav_file, buf, sizeof(buf));

	wav_file_clean(wav_file);
	log_i("wav test OK");
}

#define channels(wav_file)			wav_file->fmt->fmt_channels
#define sample_rate(wav_file)		wav_file->fmt->fmt_sample_rate
#define bit_per_sample(wav_file)	wav_file->fmt->fmt_bits_per_sample
#define data_len(wav_file)			wav_file->data->data_sz

static inline void add_blank_time(wav_file_t *wav_file, wav_file_t *new_wav_file, int len)
{
	if (len <= 0) {
		log_e("len is less than zero");
		return;
	}

    char *blank_voice = alloc_mem(len);
	wav_file_write(new_wav_file, blank_voice, len);
	free_mem(blank_voice);
}

#define WAV_MS_LEN		(15)

static inline int calc_blank_bytes(wav_file_t *wav_file, int time_s)
{
    int total_bytes = time_s * channels(wav_file) * sample_rate(wav_file) * bit_per_sample(wav_file) / 8;
	/*int blank_bytes = (total_bytes - data_len(wav_file)) / 2;*/
	int blank_bytes = total_bytes;

	switch (bit_per_sample(wav_file) / 8) {
		case 2: blank_bytes = ALIGN2(blank_bytes); break;
		case 3: blank_bytes = ALIGN3(blank_bytes); break;
		case 4: blank_bytes = ALIGN4(blank_bytes); break;

		default: log_i("bsp is error"); break;
	}
	return blank_bytes;
}

void cut_audio(const char *src_name, const char *dst_name, int time_s)
{
	wav_file_t *wav_file = wav_file_open(src_name);

	int i, ret, wav_cnt;
	i = ret = wav_cnt = 0;
	wav_file_t *cut_wav_file;

	int frame_len = channels(wav_file) * FRAME_CNT * bit_per_sample(wav_file) / 8;
	char *cut_buf = alloc_mem(frame_len);

	char dst_name_no_ext[128];
	str_get_file_name_no_extension_name(dst_name, dst_name_no_ext);

	// 去掉前面若干ms
	int cut_frame_cnt = sample_rate(wav_file) * 10 / FRAME_CNT;
	for (int j = 0; j < cut_frame_cnt; j++)
		wav_file_read(wav_file, cut_buf, frame_len);

	while (1) {
		char buf[DIR_PATH_LEN] = {0};
		sprintf(buf, "%s-%02d.wav", dst_name_no_ext, i++);
		cut_wav_file = wav_file_create(buf, channels(wav_file), sample_rate(wav_file), bit_per_sample(wav_file));

		wav_cnt = 0;
		while (1) {
			if (wav_cnt++ == sample_rate(wav_file) * time_s / FRAME_CNT)
				break;

			ret = wav_file_read(wav_file, cut_buf, frame_len);
			if (ret <= 0)
				goto wav_over;

			wav_file_write(cut_wav_file, cut_buf, ret);
		}
	
		wav_file_clean(cut_wav_file);
	}

wav_over:

	free_mem(cut_buf);
	wav_file_clean(cut_wav_file);
	wav_file_clean(wav_file);
}

#define COMBINE_WAKEUP_ASR_WAV

void stretch_appointed_time(const char *src_name, const char *dst_name, int time_s)
{
	wav_file_t *wav_file = wav_file_open(src_name);
	wav_file_t *new_wav_file = wav_file_create(dst_name, 
			channels(wav_file), sample_rate(wav_file), bit_per_sample(wav_file));

	/*add_blank_time(wav_file, new_wav_file, calc_blank_bytes(wav_file, time_s));*/
	add_blank_time(wav_file, new_wav_file, calc_blank_bytes(wav_file, 5));

#ifdef COMBINE_WAKEUP_ASR_WAV
	wav_file_t *wakeup_file = wav_file_open("wav/xiaodu-48k-jiaozhun.wav");

	char *wakeup_voice = alloc_mem(data_len(wakeup_file));
	wav_file_read(wakeup_file, wakeup_voice, data_len(wakeup_file));
	wav_file_write(new_wav_file, wakeup_voice, data_len(wakeup_file));

	free_mem(wakeup_voice);
	wav_file_clean(wakeup_file);
#endif

	char *voice = alloc_mem(data_len(wav_file));
	wav_file_read(wav_file, voice, data_len(wav_file));
	wav_file_write(new_wav_file, voice, data_len(wav_file));

	/*add_blank_time(wav_file, new_wav_file, calc_blank_bytes(wav_file, time_s));*/
	add_blank_time(wav_file, new_wav_file, calc_blank_bytes(wav_file, 5));

	free_mem(voice);
	wav_file_clean(new_wav_file);
	wav_file_clean(wav_file);
}

void save_wav_to_channel(const char *src_name, const char *dst_name)
{
	wav_file_t *wav_file = wav_file_open(src_name);
	int channels = channels(wav_file);
	wav_file_t *new_wav_file[channels];

	int bit_per_sample = bit_per_sample(wav_file) / 8;

	int frame_len = channels(wav_file) * FRAME_CNT * bit_per_sample;
	char *src_data_buf = alloc_mem(frame_len);
	char *dst_data_buf = alloc_mem(frame_len);

	char dst_name_no_ext[128];
	str_get_file_name_no_extension_name(dst_name, dst_name_no_ext);

	for (int i = 0; i < channels; i++) {
		char buf[DIR_PATH_LEN] = {0};
		sprintf(buf, "%s-%02d.wav", dst_name_no_ext, i);

		new_wav_file[i] = wav_file_create(buf, 1, sample_rate(wav_file), bit_per_sample(wav_file));
	}

	int frame_point;
	int ret;

	while (1) {
		ret = wav_file_read(wav_file, src_data_buf, frame_len);
		if (ret <= 0)
			break;

		frame_point = ret / bit_per_sample / channels;

		for (int i = 0; i < frame_point; i++) {
			for (int j = 0; j < channels; j++) {
				int src_len = i * channels * bit_per_sample + bit_per_sample * j;
				int dst_len = j * frame_point * bit_per_sample + i * bit_per_sample;

				*(dst_data_buf + dst_len + 0) = src_data_buf[src_len + 0];
				*(dst_data_buf + dst_len + 1) = src_data_buf[src_len + 1];
			}
		}

		for (int j = 0; j < channels(wav_file); j++)
			wav_file_write(new_wav_file[j], dst_data_buf + j * frame_point * bit_per_sample, frame_point * bit_per_sample);
	}

	for (int i = 0; i < channels; i++)
		wav_file_clean(new_wav_file[i]);
	wav_file_clean(wav_file);

	free_mem(src_data_buf);
	free_mem(dst_data_buf);
}

void wav_handle(const char *base_path, const char *name, unsigned char d_type, void *args)
{
    char src_name[DIR_PATH_LEN] = {0};
	const char *dst_name = NULL;

    sprintf(src_name, "%s/%s", base_path, name);
	dst_name = str_replace_substr(src_name, SRC_DIR, DST_DIR, 1);

	log_i("src_name: %s, dst_name: %s", src_name, dst_name);

	if (d_type == DT_DIR) {
		char buf[DIR_PATH_LEN] = {0};

		if (access(dst_name, F_OK) != 0) {
			sprintf(buf, "mkdir -p %s", dst_name);
			system(buf);
		}
	} else if (d_type == DT_REG) {
		cut_audio(src_name, dst_name, 20); //8
		/*stretch_appointed_time(src_name, dst_name, WAV_MS_LEN);*/
		/*save_wav_to_channel(src_name, dst_name);*/
	} else
		log_e("error");

	free_mem(dst_name);
}

static void synthetic_audio(void)
{
	if (access(DST_DIR_PATH, F_OK) != 0) {
		char buf[DIR_PATH_LEN] = {0};

		sprintf(buf, "mkdir -p %s", DST_DIR_PATH);
		system(buf);
	}

	read_file_list(SRC_DIR_PATH, wav_handle, NULL);

	log_i("succesful ...");
}

static void wav_test_init(void)
{
	handle_test_cmd_t wav_test_cmd[] = {
		{"5", wav_test},
		/*{"6", create_new_wav},*/
		{"6", synthetic_audio},
	};

	register_test_cmd(wav_test_cmd, ARRAY_NUM(wav_test_cmd));
}
DECLARE_INIT(wav_test_init);

