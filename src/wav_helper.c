/**
 *
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

static inline void _header_riff_init(riff_t *riff)
{
	strncpy(riff->riff_id, ID_RIFF, 4);
	strncpy(riff->riff_fmt, ID_WAVE, 4);

	riff->riff_sz = WAV_HEADER_LEN - 8;
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

static inline void _header_fmt_init(fmt_t *fmt, 
		int channel, int sample_rate, int bit_per_sample)
{
	strncpy(fmt->fmt_id, ID_FMT, 4);

	fmt->fmt_sz				 = FMT_SZ_LEN;
	fmt->fmt_audio_format    = FORMAT_PCM;
	fmt->fmt_channels        = channel;
	fmt->fmt_sample_rate     = sample_rate;
	fmt->fmt_byte_rate       = (channel * bit_per_sample / 8) * sample_rate;
	fmt->fmt_block_align     = channel * bit_per_sample / 8;
	fmt->fmt_bits_per_sample = bit_per_sample;
}

static inline void _header_fmt_dump(fmt_t *fmt)
{
	char fmt_id[5] = {0};

	strncpy(fmt_id, fmt->fmt_id, 4);

	log_i("fmt_id: %s",					fmt_id);
	log_i("fmt_sz: %d",					fmt->fmt_sz);
	log_i("fmt_audio_format: %d",		fmt->fmt_audio_format);
	log_i("fmt_channels: %d",			fmt->fmt_channels);
	log_i("fmt_sample_rate: %d",		fmt->fmt_sample_rate);
	log_i("fmt_byte_rate: %d",			fmt->fmt_byte_rate);
	log_i("fmt_block_align: %d",		fmt->fmt_block_align);
	log_i("fmt_bits_per_sample: %d\n",	fmt->fmt_bits_per_sample);
}

static inline void _header_data_init(data_t *data)
{
	strncpy(data->data_id, ID_DATA, 4);

	data->data_sz = 0;
}

static inline void _header_data_dump(data_t *data)
{
	char data_id[5] = {0};

	strncpy(data_id, data->data_id, 4);

	log_i("data_id: %s", data_id);
	log_i("data_sz: %d", data->data_sz);
}

static inline void fread_append_msg(wav_file_t *wav_file)
{
	append_msg_t *append_msg = alloc_mem(APPEND_MSG_T_LEN);

	fread(&append_msg->append_sz, 1, 2, wav_file->fp);
	if (append_msg->append_sz > 0)
		fread(append_msg->append_data, 1, append_msg->append_sz, wav_file->fp);

	wav_file->append_msg = append_msg;
}

static inline void _header_read(wav_file_t *wav_file)
{
	fread(wav_file->riff, 1, RIFF_T_LEN, wav_file->fp);
	fread(wav_file->fmt, 1, FMT_T_LEN, wav_file->fp);

	switch (wav_file->fmt->fmt_sz) {
		case 16:
			fread(wav_file->data, 1, DATA_T_LEN, wav_file->fp);
			break;
		case 18:
			fread_append_msg(wav_file);

			fread(wav_file->data, 1, DATA_T_LEN, wav_file->fp);
			break;
		default:
			log_i("error wav format");
			break;
	}
}

void wav_file_header_dump(wav_file_t *wav_file)
{
	long pos = ftell(wav_file->fp);

	wav_file_flush(wav_file);

	fseek(wav_file->fp, 0, SEEK_SET);

	_header_read(wav_file);

	log_i("------------------------------------");
	_header_riff_dump(wav_file->riff);
	_header_fmt_dump(wav_file->fmt);
	_header_data_dump(wav_file->data);
	log_i("------------------------------------");

	fseek(wav_file->fp, pos, SEEK_SET);
}

static inline void _header_init(wav_file_t *wav_file)
{
	wav_file->riff = alloc_mem(RIFF_T_LEN);
	wav_file->fmt = alloc_mem(FMT_T_LEN);
	wav_file->data = alloc_mem(DATA_T_LEN);

	wav_file->play_ms = 0;
}

static inline void _header_write(wav_file_t *wav_file, 
		int channel, int sample_rate, int bit_per_sample)
{
	_header_riff_init(wav_file->riff);
	_header_fmt_init(wav_file->fmt, channel, sample_rate, bit_per_sample);
	_header_data_init(wav_file->data);

	fwrite(wav_file->riff, 1, RIFF_T_LEN, wav_file->fp);
	fwrite(wav_file->fmt, 1, FMT_T_LEN, wav_file->fp);
	fwrite(wav_file->data, 1, DATA_T_LEN, wav_file->fp);
}

wav_file_t *wav_file_create(const char *path, 
		int channel, int sample_rate, int bit_per_sample)
{
	wav_file_t *wav_file = alloc_mem(WAV_FILE_LEN);

	_header_init(wav_file);

	wav_file->fp = fopen(path, "w+");

	_header_write(wav_file, channel, sample_rate, bit_per_sample);

	return wav_file;
}

wav_file_t *wav_file_open(const char *path)
{
	wav_file_t *wav_file = alloc_mem(WAV_FILE_LEN);

	_header_init(wav_file);

	wav_file->fp = fopen(path, "r");

	_header_read(wav_file);

	return wav_file;
}

void wav_file_clean(wav_file_t *wav_file)
{
	wav_file_flush(wav_file);

	fclose(wav_file->fp);	

	if (wav_file->append_msg)
		free_mem(wav_file->append_msg);

	free_mem(wav_file->riff);	
	free_mem(wav_file->fmt);	
	free_mem(wav_file->data);	
	free_mem(wav_file);	
}

void wav_file_flush(wav_file_t *wav_file)
{
	fseek(wav_file->fp, 0, SEEK_SET);
	fwrite(wav_file->riff, 1, RIFF_T_LEN, wav_file->fp);

	fseek(wav_file->fp, WAV_HEADER_LEN - DATA_T_LEN, SEEK_SET);
	fwrite(wav_file->data, 1, DATA_T_LEN, wav_file->fp);

	fflush(wav_file->fp);
}

int wav_file_write(wav_file_t *wav_file, void *data, int len)
{
	if (!(wav_file && data && len > 0))
		return -1;

	int ret = fwrite(data, 1, len, wav_file->fp);
	if (ret > 0) {
		wav_file->riff->riff_sz += ret;
		wav_file->data->data_sz += ret;
	}

	return ret;
}

int wav_file_read(wav_file_t *wav_file, void *data, int len)
{
	if (!(wav_file && data && len > 0))
		return -1;

	return fread(data, 1, len, wav_file->fp);
}

