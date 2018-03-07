/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa_record_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/03 2018 20:44
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/03 2018      create the file
 * 
 *     last modified: 06/03 2018 20:44
 */
#include <stdio.h>
#include <stdlib.h>

#define ALSA_RECORD_INTERFACE_GB
#include "alsa_record_interface.h"
#undef ALSA_RECORD_INTERFACE_GB

#define DEBUG(x,y...)	\
	do {				\
		printf("[%s : %s : %d]", __FILE__, __func__, __LINE__); \
		printf(x, ##y); \
		printf("\n");	\
	} while(0)

#define ERROR(x,y...)	\
	do {				\
		printf("[%s : %s : %d]", __FILE__, __func__, __LINE__); \
		printf(x, ##y); \
		printf("\n");	\
	} while(0)

snd_pcm_t *alsa_record_get_handle(void)
{
	int err;
	unsigned int rate = SAMPLE_RATE;
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

	if ((err = snd_pcm_open(&capture_handle, DEV_NAME, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		ERROR("err snd_pcm_open, device-name: %s", DEV_NAME);
		goto ERR_OPEN_FAILD;
	}

	snd_pcm_hw_params_alloca(&hw_params);

	if (snd_pcm_hw_params_any(capture_handle, hw_params) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	if (snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
		ERROR("snd_pcm_hw_params_set_access faild");
		goto ERR_HANDLE;
	}

	if (snd_pcm_hw_params_set_format(capture_handle, hw_params, format) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	if (snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate, 0) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	if (snd_pcm_hw_params_set_channels(capture_handle, hw_params, CHANNELS) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	if (snd_pcm_hw_params(capture_handle, hw_params) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	if ((err = snd_pcm_prepare(capture_handle)) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	/*snd_pcm_hw_params_malloc(&hw_params);*/
	/*snd_pcm_hw_params_free(hw_params);*/

	return capture_handle;

ERR_HANDLE:
	snd_pcm_close(capture_handle);

ERR_OPEN_FAILD:
	return NULL;
}

void alsa_record_put_handle(snd_pcm_t *capture_handle)
{
	snd_pcm_close(capture_handle);
}

int alsa_record_read_pcm(snd_pcm_t *capture_handle, void *buf, int len)
{
	assert(capture_handle);
	assert(buf);
	int buf_len = len / FRAMES_SIZE;

	int ret = snd_pcm_readi(capture_handle, buf, buf_len);
	if (ret != buf_len) {
		printf ("read from audio interface failed (%s)\n", snd_strerror(ret));
		exit (1);
	}

	return ret;
}

