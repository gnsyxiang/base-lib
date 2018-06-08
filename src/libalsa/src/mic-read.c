/**
 * 
 * Release under GPLv2.
 * 
 * @file    mic-read.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/05 2018 17:41
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/05 2018      create the file
 * 
 *     last modified: 02/05 2018 17:41
 */
#include <stdio.h>

#include <ringbuf.h>
#include <log_helper.h>
#include <pthread_helper.h>

#define MIC_READ_GB
#include "mic-read.h"
#undef MIC_READ_GB
#include "alsa-record.h"

static record_handle_t *mic_read_alsa_fd;
static pringbuf_t mic_read_ringbuf;

static void open_mic(void)
{
	record_params_t record_params;

	record_params.snd_dev_name	= DEV_NAME;
	record_params.format		= snd_pcm_format_value(BIT_PER_SAMPLE_STR);
	record_params.channels		= CHANNELS;
	record_params.sample_rate	= SAMPLE_RATE;
	record_params.period_time	= PERIOD_TIME;

	log_i("BIT_PER_SAMPLE_STR: %s", BIT_PER_SAMPLE_STR);

	mic_read_alsa_fd = alsa_record_init(record_params);
	if (!mic_read_alsa_fd)
		log_e("mic_read_alsa_fd is NULL");
}

static void *mic_read_loop(void *args)
{
	static mic_data_t buf[FRAME_CNT];

	while (1) {
		if (alsa_record_get_data(mic_read_alsa_fd, buf)) {
			usleep(10 * 1000);
			continue;
		}

		ringbuf_in(mic_read_ringbuf, buf, sizeof(buf));
	}

	return NULL;
}

void mic_read_get_data(void *buf, int len)
{
	/*log_i("ringbuf size: %d, remain size: %d, use size: %d",*/
			/*mic_read_ringbuf->size, mic_read_ringbuf->remain_size, */
			/*mic_read_ringbuf->size - mic_read_ringbuf->remain_size);*/

	ringbuf_out(mic_read_ringbuf, buf, len);
}

void mic_read_init(void)
{
	open_mic();

	mic_read_ringbuf = ringbuf_init(MIC_READ_QUEUE_LEN);
	if (!mic_read_ringbuf)
		log_e("ringbuf init faild");

	create_a_attached_thread(NULL, mic_read_loop, NULL);
}

void mic_read_clean(void)
{
	ringbuf_destroy(mic_read_ringbuf);
	alsa_record_clean(mic_read_alsa_fd);
}

void mic_read_pause(void)
{
	alsa_record_pause(mic_read_alsa_fd);
}

void mic_read_resume(void)
{
	alsa_record_resume(mic_read_alsa_fd);
}

