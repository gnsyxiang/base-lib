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

snd_pcm_t *mic_init(void)
{
	int err;
	unsigned int rate = SAMPLE_RATE;
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

	if ((err = snd_pcm_open (&capture_handle, DEV_NAME, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf (stderr, "cannot open audio device %s (%s)\n", DEV_NAME, snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n", snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, format)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n", snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n", snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, CHANNELS)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n", snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n", snd_strerror (err));
		exit (1);
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (capture_handle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror (err));
		exit (1);
	}

	return capture_handle;
}

void mic_clean(snd_pcm_t *capture_handle)
{
	snd_pcm_close (capture_handle);
}

int mic_read(snd_pcm_t *capture_handle, void *buf, int len)
{
	assert(capture_handle);
	assert(buf);
	int buf_len = len / FRAMES_SIZE;

	int ret = snd_pcm_readi (capture_handle, buf, buf_len);
	if (ret != buf_len) {
		fprintf (stderr, "read from audio interface failed (%s)\n", snd_strerror(ret));
		exit (1);
	}

	return ret;
}

