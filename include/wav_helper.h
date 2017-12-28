/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    wav_helper.h
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
#ifndef _BASE_LIB_WAV_HELPER_H_
#define _BASE_LIB_WAV_HELPER_H_

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_WAV_HELPER_GB
#define BASE_LIB_WAV_HELPER_EX extern
#else
#define BASE_LIB_WAV_HELPER_EX
#endif

#define WAV_FILE_PATH_LEN (256)

typedef struct _wav_header_tag {
    char  riff_id[4];			//"RIFF"
    int   riff_sz;				// RIFF chunk data size,exclude riff_id[4] and riff_datasize,total - 8
    char  riff_fmt[4];          // "WAVE"

    char  fmt_id[4];            // "fmt "
    int   fmt_sz;               // fmt chunk data size,16 for pcm
    short fmt_audio_format;     // 1 for PCM
    short fmt_channels;         // 1(mono) or 2(stereo)
    int   fmt_sample_rate;      // samples per second
    int   fmt_byte_rate;        // sample_rate * channels * bit_per_sample / 8
    short fmt_block_align;      // number bytes per sample, bit_per_sample * channels / 8
    short fmt_bits_per_sample;  // bits of each sample(8,16,32).

    char  data_id[4];           // "data"
    int   data_sz;              // data chunk size,pcm_size - 44
}wav_header_t;

typedef struct _wav_file_tag {
    wav_header_t *wav_header;		// wav header
    int			 play_ms;           // data_size / fmt_byte_rate
    FILE		 *file;
}wav_file_t;

typedef struct _wav_file_param_tag {
	char path[WAV_FILE_PATH_LEN];
	char file_mode[5];
	int channels;
	int sample_rate;
	int bit_per_sample;
}wav_file_param_t;

#define WAV_HEADER_LEN		(sizeof(wav_header_t))
#define WAV_FILE_LEN		(sizeof(wav_file_t))
#define WAV_FILE_PARAM_LEN	(sizeof(wav_file_param_t))

BASE_LIB_WAV_HELPER_EX wav_file_t *wav_file_create(wav_file_param_t *wav_file_param);
BASE_LIB_WAV_HELPER_EX wav_file_t *wav_file_open(wav_file_param_t *wav_file_param);
BASE_LIB_WAV_HELPER_EX void wav_file_clean(wav_file_t *wav_file);

BASE_LIB_WAV_HELPER_EX int wav_file_write(wav_file_t *wav_file, void *data, int len);
BASE_LIB_WAV_HELPER_EX int wav_file_read(wav_file_t *wav_file, void *data, int len);

BASE_LIB_WAV_HELPER_EX void wav_header_dump(wav_file_t *wav_file);
BASE_LIB_WAV_HELPER_EX void wav_file_flush(wav_file_t *wav_file);
BASE_LIB_WAV_HELPER_EX void wav_file_seek(wav_file_t *wav_file, long offset, int whence);

BASE_LIB_WAV_HELPER_EX void wav_file_rewind(wav_file_t *wav_file);
BASE_LIB_WAV_HELPER_EX int wav_file_over(wav_file_t *wav_file);

#ifdef  __cplusplus
}
#endif

#endif /* _WAV_HELPER_H */

