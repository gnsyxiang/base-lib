/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa-record-test.c
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
#include <stdio.h>

#include <log_helper.h>

#include "mic-read.h"

#define SAVE_PCM

#ifdef SAVE_PCM
#include <wav_helper.h>
#define DEMO_WAV_NAME		"test.wav"
#define BIT_PER_SAMPLE		get_bit_per_sample(BIT_PER_SAMPLE_STR)
#define WAV_CNT_LEN			(100)

static wav_file_t *demo_wav_file;
static int demo_wav_cnt;
#endif

int main(int argc, char *argv[])
{
	static mic_data_t mic_data[FRAME_CNT];

	mic_read_init();

#ifdef SAVE_PCM
	demo_wav_file = wav_file_create(DEMO_WAV_NAME, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);
#endif

	while (1) {
		mic_read_get_data(mic_data, sizeof(mic_data));

#ifdef SAVE_PCM
		if (demo_wav_cnt++ < WAV_CNT_LEN)
			wav_file_write(demo_wav_file, mic_data, sizeof(mic_data));

		log_i("--demo_wav_cnt: %d", demo_wav_cnt);

		if (demo_wav_cnt == WAV_CNT_LEN) {
			printf("--cb--over------demo_wav_cnt: %d \n", demo_wav_cnt);
			wav_file_clean(demo_wav_file);
		}
#endif
	}

	mic_read_clean();

	return 0;
}

