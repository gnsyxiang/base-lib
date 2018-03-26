/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa-record.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/03 2018 16:35
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/03 2018      create the file
 * 
 *     last modified: 26/03 2018 16:35
 */
#ifndef __ALSA_RECORD_H_
#define __ALSA_RECORD_H_

#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <locale.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "alsa/asoundlib.h"
#include <assert.h>

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

/**
 * @brief Recording handle.
 */
typedef struct {
	/**
	 * @brief Alsa handle.
	 */
	snd_pcm_t *handle;
	/**
	 * @brief Data format.
	 */
	snd_pcm_format_t format;
	/**
	 * @brief The number of channels.
	 */
	uint16_t channels;
	/**
	 * @brief frequence of sample
	 */
	uint32_t sample_rate;
	/**
	 * @brief The number of bit per sample point.
	 */
	size_t bits_per_sample;
	/**
	 * @brief The number of bit per frame.
	 */
	size_t bits_per_frame;
	/**
	 * @brief The Idle frame number that can be read.
	 */
	snd_pcm_uframes_t frame_num;
} record_handle_t;

/**
 * @brief Recording parameters.
 */
typedef struct {

	/**
	 * @brief Sound card device name
	 */
	const char *snd_dev_name;

	/*
	 * Specifies data format.
	 * SND_PCM_FORMAT_S16_LE;
	 * SND_PCM_FORMAT_S8;
	 * SND_PCM_FORMAT_UNKNOWN;
	 * All references at sound/asound.h
	 */
	snd_pcm_format_t format;
	/**
	 * @brief Specifies the number of channels.
	 */
	uint16_t channels;
	/**
	 * @brief frequence of sample
	 */
	uint32_t sample_rate;
} record_params_t;

/**
 * @brief Recording result.
 */
typedef struct {
	/**
	 * @brief Point to the recording data.
	 */
	uint8_t *data_buf;
	/**
	 * @brief The size of recording data.
	 */
	size_t data_buf_size;
} record_result_t;

/**
 * @brief Get the handle for the recording.
 *
 * @param record_params Recording parameters.
 *
 * @return Record handle on success otherwise NULL is returned.
 */
record_handle_t *alsa_get_record_handle(record_params_t record_params);

/**
 * @brief Get the recording data, the result in the record_result.
 * record_result must be free after use.
 *
 * @param record_handle
 *
 * @return Return record_result, on error record_result.data_buf is NULL.
 */
record_result_t alsa_record_get_data(record_handle_t * record_handle);

/**
 * @brief Recording by time length.
 * record_result must be free after use.
 *
 * @param record_handle
 * @param second
 *
 * @return Return record_result, on error record_result.data_buf is NULL.
 */
record_result_t alsa_record_by_time(record_handle_t * record_handle, uint32_t second);

/**
 * @brief Recording by data size.
 * record_result must be free after use.
 *
 * @param record_handle
 * @param size
 *
 * @return Return record_result, on error record_result.data_buf is NULL.
 */
record_result_t alsa_record_by_size(record_handle_t * record_handle, uint32_t size);

/**
 * @brief Release record handle after call alsa_get_record_handle.
 *
 * @param record_handle
 */
void alsa_put_record_handle(record_handle_t * record_handle);

/**
 * @brief Free no longer in use record_result
 *
 * @param record_result record_result
 */
void alsa_free_record_result(record_result_t record_result);

#endif /* end __ALSA_RECORD_H_ */

