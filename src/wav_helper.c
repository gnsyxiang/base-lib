/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    wav_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    22/12 2017 20:11
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        22/12 2017      create the file
 * 
 *     last modified: 22/12 2017 20:11
 */
#include <stdio.h>
#include <string.h>

#define BASE_LIB_WAV_HELPER_GB
#include "wav_helper.h"
#undef BASE_LIB_WAV_HELPER_GB

#include "heap_memory_helper.h"

#define ID_RIFF "RIFF"
#define ID_WAVE "ID_WAVE"
#define ID_FMT  "fmt "
#define ID_DATA "data"

#define FORMAT_PCM 1

static wav_header_t *wav_header_init(wav_file_param_t *wav_file_param)
{
	int channels = wav_file_param->channels;
	int sample_rate = wav_file_param->sample_rate;
	int bits_per_sample = wav_file_param->bit_per_sample;

	wav_header_t *wav_header = safer_malloc(WAV_HEADER_LEN);	

	strcpy(wav_header->riff_id, ID_RIFF);
	wav_header->riff_sz = 0;
	strcpy(wav_header->riff_fmt, ID_WAVE);

	strcpy(wav_header->fmt_id, ID_FMT);
	wav_header->fmt_sz = 16;
	wav_header->fmt_audio_format = FORMAT_PCM;
	wav_header->fmt_channels = channels;
	wav_header->fmt_sample_rate = sample_rate;
	wav_header->fmt_byte_rate = (channels * bits_per_sample / 8) * sample_rate;
	wav_header->fmt_block_align = channels * bits_per_sample / 8;
	wav_header->fmt_bits_per_sample = bits_per_sample;

	strcpy(wav_header->data_id, ID_DATA);
	wav_header->data_sz = 0;

	return wav_header;
}

wav_file_t *wav_file_init(wav_file_param_t *wav_file_param)
{
	wav_file_t *wav_file = safer_malloc(WAV_FILE_LEN);

	wav_file->wav_header = wav_header_init(wav_file_param);
	wav_file->play_ms = 0;
	wav_file->file = fopen(wav_file_param->path, "w+");

	return wav_file;
}

void wav_file_clean(wav_file_t *wav_file)
{
	safer_free(wav_file->wav_header);
	fclose(wav_file->file);
	safer_free(wav_file);
}

void wav_header_write(wav_file_t *wav_file, int riff_sz, int data_sz)
{
	wav_file->wav_header->riff_sz = riff_sz;
	wav_file->wav_header->data_sz = data_sz;
}


