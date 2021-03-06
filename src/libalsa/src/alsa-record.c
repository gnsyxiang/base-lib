/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa-record.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/03 2018 16:34
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/03 2018      create the file
 * 
 *     last modified: 26/03 2018 16:34
 */
#include <stdio.h>

#include <log_helper.h>

#define ALSA_RECORD_GB
#include "alsa-record.h"
#undef ALSA_RECORD_GB

static int record_set_params(record_handle_t * r_handle, record_params_t record_params)
{
	snd_pcm_hw_params_t *hwparams;
	snd_pcm_uframes_t alsa_buffer_size;
	bl_uint32_t exact_rate;
	bl_uint32_t buffer_time;

	/* Allocate the snd_pcm_hw_params_t structure on the stack. */
	snd_pcm_hw_params_alloca(&hwparams);

	/* Init hwparams with full configuration space */
	if (snd_pcm_hw_params_any(r_handle->handle, hwparams) < 0) {
		log_e("snd_pcm_hw_params_any");
		goto ERR_SET_PARAMS;
	}

	if (snd_pcm_hw_params_set_access(r_handle->handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
		log_e("snd_pcm_hw_params_set_access");
		goto ERR_SET_PARAMS;
	}

	if (snd_pcm_hw_params_set_format(r_handle->handle, hwparams, record_params.format) < 0) {
		log_e("snd_pcm_hw_params_set_format");
		goto ERR_SET_PARAMS;
	}
	r_handle->format = record_params.format;

	/* Set number of channels */
	if (snd_pcm_hw_params_set_channels(r_handle->handle, hwparams, record_params.channels) < 0) {
		log_e("snd_pcm_hw_params_set_channels");
		goto ERR_SET_PARAMS;
	}
	r_handle->channels = record_params.channels;

	/* Set sample rate. If the exact rate is not supported */
	/* by the hardware, use nearest possible rate.		 */
	exact_rate = record_params.sample_rate;
	if (snd_pcm_hw_params_set_rate_near(r_handle->handle, hwparams, &exact_rate, 0) < 0) {
		log_e("snd_pcm_hw_params_set_rate_near");
		goto ERR_SET_PARAMS;
	}
	if (record_params.sample_rate != exact_rate) {
		log_e("The rate %d Hz is not supported by your hardware. ==> Using %d Hz instead.",
			record_params.sample_rate, exact_rate);
	}
	r_handle->sample_rate = exact_rate;

	if (snd_pcm_hw_params_get_buffer_time_max(hwparams, &buffer_time, 0) < 0) {
		log_e("snd_pcm_hw_params_get_buffer_time_max");
		goto ERR_SET_PARAMS;
	}

	if (!record_params.period_time) {
		if (buffer_time > 500000) 
			buffer_time = 500000;
		record_params.period_time = buffer_time / 4;
	} else {
		buffer_time = record_params.period_time * 5;
	}

	if (snd_pcm_hw_params_set_buffer_time_near(r_handle->handle, hwparams, &buffer_time, 0) < 0) {
		log_e("snd_pcm_hw_params_set_buffer_time_near");
		goto ERR_SET_PARAMS;
	}

	if (snd_pcm_hw_params_set_period_time_near(r_handle->handle, hwparams, &record_params.period_time, 0) < 0) {
		log_e("snd_pcm_hw_params_set_period_time_near");
		goto ERR_SET_PARAMS;
	}

	/* Set hw params */
	if (snd_pcm_hw_params(r_handle->handle, hwparams) < 0) {
		log_e("snd_pcm_hw_params(handle, params)");
		goto ERR_SET_PARAMS;
	}

	snd_pcm_hw_params_get_period_size(hwparams, &r_handle->frame_num, 0);
	snd_pcm_hw_params_get_buffer_size(hwparams, &alsa_buffer_size);
	if (r_handle->frame_num == alsa_buffer_size) {
		log_e(("Can't use period equal to buffer size (%lu == %lu)"), r_handle->frame_num, alsa_buffer_size);
		goto ERR_SET_PARAMS;
	}

	//通过fomart获取每sample的bit数
	r_handle->bits_per_sample = snd_pcm_format_physical_width(record_params.format);

	//把frame定义为一个sample的数据，包括每个channel
	r_handle->bits_per_frame = r_handle->bits_per_sample * record_params.channels;

	//snd_pcm_hw_params_free (hwparams);

	return 0;

ERR_SET_PARAMS:
	return -1;
}

record_handle_t *alsa_record_init(record_params_t record_params)
{
	record_handle_t *record_handle;
	record_handle = (record_handle_t *)malloc(sizeof(record_handle_t));
	memset(record_handle, 0x0, sizeof(record_handle_t));

	if (snd_pcm_open(&(record_handle->handle), record_params.snd_dev_name, SND_PCM_STREAM_CAPTURE, 0) < 0) {
		log_e("snd_pcm_open [ %s]", record_params.snd_dev_name);
		free(record_handle);
		return NULL;
	}

	if (record_set_params(record_handle, record_params) < 0) {
		log_e("set_snd_pcm_params");
		free(record_handle);
		return NULL;
	}
	return record_handle;
}

void alsa_record_clean(record_handle_t *record_handle)
{
	snd_pcm_drain(record_handle->handle);
	snd_pcm_close(record_handle->handle);
	free(record_handle);
}

ssize_t read_pcm(record_handle_t *record_handle, void *buf)
{
	ssize_t r;
	size_t result = 0;
	size_t count = record_handle->frame_num;

	while (count > 0) {
		//录count个frame到data中
		r = snd_pcm_readi(record_handle->handle, buf, count);

		if (r == -EAGAIN || (r >= 0 && (size_t)r < count)) {
			snd_pcm_wait(record_handle->handle, 1000);
		} else if (r == -EPIPE) {
			snd_pcm_prepare(record_handle->handle);
			log_e("<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
		} else if (r == -ESTRPIPE) {
			log_e("<<<<<<<<<<<<<<< Need suspend >>>>>>>>>>>>>>>\n");
		} else if (r < 0) {
			log_e("snd_pcm_readi: [%s]", snd_strerror(r));
			return -1;
		}

		if (r > 0) {
			result += r;
			count -= r;
		}
	}
	return result;
}

void alsa_record_get_data(record_handle_t *record_handle, void *buf)
{
	if (read_pcm(record_handle, buf) != record_handle->frame_num)
		printf("some thing is wrong while read mic data\n");
}

