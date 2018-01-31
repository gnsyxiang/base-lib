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

#if 1
static wav_file_param_t *wav_file_param_init(const char *path, int channels, int sample_rate, int bit_per_sample)
{
	wav_file_param_t *wav_file_param = alloc_mem(WAV_FILE_PARAM_LEN);

	strcpy(wav_file_param->path, path);

	wav_file_param->channels		= channels;
	wav_file_param->sample_rate		= sample_rate;
	wav_file_param->bit_per_sample	= bit_per_sample;	

	return wav_file_param;
}

static inline riff_t *_header_riff_init(void)
{
	riff_t *riff = alloc_mem(RIFF_T_LEN);

	strcpy(riff->riff_id, ID_RIFF);
	strcpy(riff->riff_fmt, ID_WAVE);

	riff->riff_sz = WAV_HEADER_LEN - 8;

	return riff;
}

static inline void _header_riff_dump(riff_t *riff)
{
	char riff_id[5] = {0};
	char riff_fmt[5] = {0};

	strncpy(riff_id,  riff->riff_id, 4);
	strncpy(riff_fmt, riff->riff_fmt, 4);

	log_i("riff_id: %s",    riff_id);
	log_i("riff_sz: %d",    riff->riff_sz);
	log_i("riff_fmt: %s\n", riff_fmt);
}

static inline fmt_t *_header_fmt_init(wav_file_param_t *wav_file_param)
{
	int channels	    = wav_file_param->channels;
	int sample_rate     = wav_file_param->sample_rate;
	int bits_per_sample = wav_file_param->bit_per_sample;

	fmt_t *fmt = alloc_mem(FMT_T_LEN);

	strcpy(fmt->fmt_id, ID_FMT);
	fmt->fmt_sz			   = 16;
	fmt->fmt_audio_format    = FORMAT_PCM;
	fmt->fmt_channels        = channels;
	fmt->fmt_sample_rate     = sample_rate;
	fmt->fmt_byte_rate       = (channels * bits_per_sample / 8) * sample_rate;
	fmt->fmt_block_align     = channels * bits_per_sample / 8;
	fmt->fmt_bits_per_sample = bits_per_sample;

	return fmt;
}

static inline data_t *_header_data_init(void)
{
	data_t *data = alloc_mem(DATA_T_LEN);

	strcpy(data->data_id, ID_DATA);
	data->data_sz = 0;

	return data;
}

void wav_file_header_dump(wav_file_t *wav_file)
{
	riff_t riff;
	fseek(wav_file->fp, 0, SEEK_SET);

	fread(&riff, 1, RIFF_T_LEN, wav_file->fp);
	_header_riff_dump(&riff);
}

	static int cnt = 0;
static void wav_header_update(wav_file_t *wav_file, int len)
{
	cnt++;
	wav_file->riff->riff_sz += len;
	wav_file->data->data_sz += len;
}

wav_file_t *wav_file_create(const char *path, int channel, int sample_rate, int bit_per_sample)
{
	wav_file_param_t *wav_file_param = wav_file_param_init(path, channel, sample_rate, bit_per_sample);

	strcpy(wav_file_param->file_mode, "w+");

	wav_file_t *wav_file = malloc_mem(WAV_FILE_LEN);

	wav_file->riff		 = _header_riff_init();
	wav_file->fmt		 = _header_fmt_init(wav_file_param);
	wav_file->data       = _header_data_init();

	wav_file->play_ms = 0;
	wav_file->fp = fopen(wav_file_param->path, wav_file_param->file_mode);

	free_mem(wav_file_param);

	fwrite(wav_file->riff, 1, RIFF_T_LEN, wav_file->fp);
	fwrite(wav_file->fmt, 1, FMT_T_LEN, wav_file->fp);
	fwrite(wav_file->data, 1, DATA_T_LEN, wav_file->fp);

	return wav_file;
}

void wav_file_flush(wav_file_t *wav_file)
{
	fseek(wav_file->fp, 0, SEEK_SET);

	fwrite(wav_file->riff, 1, RIFF_T_LEN, wav_file->fp);

	fseek(wav_file->fp, WAV_HEADER_LEN - 4, SEEK_SET);
	fwrite(&wav_file->data->data_sz, 1, 4, wav_file->fp);

	fflush(wav_file->fp);
}

void wav_file_clean(wav_file_t *wav_file)
{
	wav_file_flush(wav_file);
	fclose(wav_file->fp);	

	free_mem(wav_file->riff);	
	free_mem(wav_file->fmt);	
	free_mem(wav_file->data);	
	free_mem(wav_file);	

	printf("cnt: %d \n", cnt);
}

int wav_file_write(wav_file_t *wav_file, void *data, int len)
{
	int ret = fwrite(data, 1, len, wav_file->fp);
	if (ret > 0)
		wav_header_update(wav_file, ret);

	return ret;
}

#endif
