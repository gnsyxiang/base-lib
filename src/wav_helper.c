/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
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

#include "mem_helper.h"
#include "log_helper.h"

#define ID_RIFF "RIFF"
#define ID_WAVE "WAVE"
#define ID_FMT  "fmt "
#define ID_DATA "data"

#define FORMAT_PCM 1

static wav_header_t *wav_header_init(wav_file_param_t *wav_file_param)
{
	int channels = wav_file_param->channels;
	int sample_rate = wav_file_param->sample_rate;
	int bits_per_sample = wav_file_param->bit_per_sample;

	wav_header_t *wav_header = malloc_mem(WAV_HEADER_LEN);	
	
	if (!channels || !sample_rate || !bits_per_sample) {
		return wav_header;
	}

	strcpy(wav_header->riff_id, ID_RIFF);
	wav_header->riff_sz = WAV_HEADER_LEN - 8;
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

static wav_file_t *new_wav_file_t(wav_file_param_t *wav_file_param)
{
	wav_file_t *wav_file = malloc_mem(WAV_FILE_LEN);

	wav_file->wav_header = wav_header_init(wav_file_param);
	wav_file->play_ms = 0;
	wav_file->file = fopen(wav_file_param->path, wav_file_param->file_mode);

	return wav_file;
}

void wav_header_dump(wav_file_t *wav_file)
{
	char riff_id[5] = {0};
	char riff_fmt[5] = {0};
	char fmt_id[5] = {0};
	char data_id[5] = {0};
	wav_header_t wav_header;

	fseek(wav_file->file, 0, SEEK_SET);

	fread(&wav_header, 1, WAV_HEADER_LEN, wav_file->file);

	strncpy(riff_id, wav_header.riff_id, 4);
	strncpy(riff_fmt, wav_header.riff_fmt, 4);
	strncpy(fmt_id, wav_header.fmt_id, 4);
	strncpy(data_id, wav_header.data_id, 4);

	log_i("---------------------------");
	log_i("riff_id: %s", riff_id);
	log_i("riff_sz: %d", wav_header.riff_sz);
	log_i("riff_fmt: %s\n", riff_fmt);

	log_i("fmt_id: %s", fmt_id);
	log_i("fmt_sz: %d", wav_header.fmt_sz);
	log_i("fmt_audio_format: %d", wav_header.fmt_audio_format);
	log_i("fmt_channels: %d", wav_header.fmt_channels);
	log_i("fmt_sample_rate: %d", wav_header.fmt_sample_rate);
	log_i("fmt_byte_rate: %d", wav_header.fmt_byte_rate);
	log_i("fmt_block_align: %d", wav_header.fmt_block_align);
	log_i("fmt_bits_per_sample: %d\n", wav_header.fmt_bits_per_sample);

	log_i("data_id: %s", data_id);
	log_i("data_sz: %d", wav_header.data_sz);
	log_i("---------------------------");
}

static void wav_header_write(wav_file_t *wav_file, int len)
{
	wav_file->wav_header->riff_sz += len;
	wav_file->wav_header->data_sz += len;
}

void wav_file_flush(wav_file_t *wav_file)
{
	fseek(wav_file->file, 0, SEEK_SET);
	fwrite(wav_file->wav_header, 1, WAV_HEADER_LEN, wav_file->file);
	fflush(wav_file->file);
}

void wav_file_seek(wav_file_t *wav_file, long offset, int whence)
{
	fseek(wav_file->file, offset, whence);
}

wav_file_t *wav_file_create(wav_file_param_t *wav_file_param)
{
	strcpy(wav_file_param->file_mode, "w+");

	wav_file_t *wav_file = new_wav_file_t(wav_file_param);

	fwrite(wav_file->wav_header, 1, WAV_HEADER_LEN, wav_file->file);

	return wav_file;
}

wav_file_t *wav_file_open(const char *path)
{
	wav_file_param_t wav_file_param;

	strcpy(wav_file_param.file_mode, "r");
	strcpy(wav_file_param.path, path);

	wav_file_t *wav_file = new_wav_file_t(&wav_file_param);

	fread(wav_file->wav_header, 1, WAV_HEADER_LEN, wav_file->file);

	return wav_file;
}

void wav_file_clean(wav_file_t *wav_file)
{
	wav_file_flush(wav_file);
	fclose(wav_file->file);

	free_mem(wav_file->wav_header);
	free_mem(wav_file);
}

int wav_file_write(wav_file_t *wav_file, void *data, int len)
{
	int ret;

	if (!(wav_file && data && len > 0))
		return -1;

	ret = fwrite(data, 1, len, wav_file->file);
	if (ret > 0)
		wav_header_write(wav_file, ret);

	return ret;
}

int wav_file_read(wav_file_t *wav_file, void *data, int len)
{
	int ret;

	if (!(wav_file && data && len > 0))
		return -1;

	ret = fread(data, 1, len, wav_file->file);

	return ret;
}

