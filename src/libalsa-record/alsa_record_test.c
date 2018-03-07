/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa_record_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/03 2018 20:55
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/03 2018      create the file
 * 
 *     last modified: 06/03 2018 20:55
 */
#include <stdio.h>

#include "alsa_record_interface.h"
#include "wav_helper.h"

void save_pcm_to_wav(snd_pcm_t *capture_handle)
{
	int i;
	char *buffer;
	int frame_len = FRAMES_SIZE * FRAMES_CNT;

	printf("frame_len: %d \n", frame_len);

	buffer = malloc(frame_len);

	wav_file_t *new_wav_file = wav_file_create("test.wav", CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

	for (i = 0; i < 200; ++i) {
		alsa_record_read_pcm(capture_handle, buffer, frame_len);

		printf("i: %d \n", i);

		wav_file_write(new_wav_file, buffer, frame_len);
	}

	wav_file_clean(new_wav_file);
	free(buffer);

	fprintf(stdout, "buffer freed\n");
}

int main(void)
{
	snd_pcm_t * capture_handle = alsa_record_get_handle();

	save_pcm_to_wav(capture_handle);

	alsa_record_put_handle(capture_handle);

	return 0;
}

