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

#ifdef __cplusplus
extern "C" {
#endif

#include <alsa/asoundlib.h>
#include <typedef_helper.h>

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
	bl_uint16_t channels;
	/**
	 * @brief frequence of sample
	 */
	bl_uint32_t sample_rate;
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
	bl_uint16_t channels;
	/**
	 * @brief frequence of sample
	 */
	bl_uint32_t sample_rate;
	bl_uint32_t period_time;
} record_params_t;

record_handle_t *alsa_record_init(record_params_t record_params);
void alsa_record_clean(record_handle_t * record_handle);

void alsa_record_get_data(record_handle_t *record_handle, void *buf);

#ifdef __cplusplus
}
#endif

#endif /* end __ALSA_RECORD_H_ */

