/**
 *
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

typedef struct _riff_tag {
    char  riff_id[4];			//"RIFF"
    int   riff_sz;				// RIFF chunk data size,exclude riff_id[4] and riff_datasize,total - 8
    char  riff_fmt[4];          // "WAVE"
}riff_t;
#define RIFF_T_LEN (sizeof(riff_t))

typedef struct _fmt_tag {
    char  fmt_id[4];            // "fmt "
    int   fmt_sz;               // fmt chunk data size,16 for pcm
    short fmt_audio_format;     // 1 for PCM
    short fmt_channels;         // 1(mono) or 2(stereo)
    int   fmt_sample_rate;      // samples per second
    int   fmt_byte_rate;        // sample_rate * channels * bit_per_sample / 8
    short fmt_block_align;      // number bytes per sample, bit_per_sample * channels / 8
    short fmt_bits_per_sample;  // bits of each sample(8,16,32).
}fmt_t;
#define FMT_T_LEN	(sizeof(fmt_t))

typedef struct _append_msg_tag {
	short append_sz;
	char *append_data;
}append_msg_t;
#define APPEND_MSG_T_LEN	(sizeof(append_msg_t))

typedef struct _fact_tag {
	char fact_id[4];
	int fact_sz;
	char fact_data[4];
}fact_t;
#define FACT_T_LEN	(sizeof(fact_t))

typedef struct _data_tag {
    char  data_id[4];           // "data"
    int   data_sz;              // data chunk size,pcm_size - 44
}data_t;
#define DATA_T_LEN	(sizeof(data_t))

typedef struct _wav_file_tag {
	riff_t	     *riff;
	fmt_t	     *fmt;
	append_msg_t *append_msg;
	fact_t	     *fact;
	data_t	     *data;

    int		play_ms;           // data_size / fmt_byte_rate
    FILE	*fp;
}wav_file_t;
#define WAV_FILE_LEN		(sizeof(wav_file_t))

#define WAV_HEADER_LEN		(RIFF_T_LEN + FMT_T_LEN + DATA_T_LEN)

typedef struct _wav_file_param_tag {
	char path[WAV_FILE_PATH_LEN];
	char file_mode[5];
	int channels;
	int sample_rate;
	int bit_per_sample;
}wav_file_param_t;

#define WAV_FILE_PARAM_LEN	(sizeof(wav_file_param_t))

BASE_LIB_WAV_HELPER_EX wav_file_t *wav_file_create(const char *path, int channel, int sample_rate, int bit_per_sample);
BASE_LIB_WAV_HELPER_EX wav_file_t *wav_file_open(const char *path);
BASE_LIB_WAV_HELPER_EX void wav_file_clean(wav_file_t *wav_file);

BASE_LIB_WAV_HELPER_EX int wav_file_write(wav_file_t *wav_file, void *data, int len);
BASE_LIB_WAV_HELPER_EX int wav_file_read(wav_file_t *wav_file, void *data, int len);

BASE_LIB_WAV_HELPER_EX void wav_file_header_dump(wav_file_t *wav_file);
BASE_LIB_WAV_HELPER_EX void wav_file_flush(wav_file_t *wav_file);
BASE_LIB_WAV_HELPER_EX void wav_file_seek(wav_file_t *wav_file, long offset, int whence);

BASE_LIB_WAV_HELPER_EX void wav_file_rewind(wav_file_t *wav_file);
BASE_LIB_WAV_HELPER_EX int wav_file_over(wav_file_t *wav_file);

#ifdef  __cplusplus
}
#endif

#endif /* _WAV_HELPER_H */

