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

#include <hex_helper.h>
#include <log_helper.h>

#include "mic-read.h"

#define SAVE_PCM
#define PARSE_XIAOMI_DATA

typedef struct mic_data_dst {
	short amic[CHANNELS];
}mic_data_dst_t;

#ifdef SAVE_PCM
#include <wav_helper.h>
#define DEMO_WAV_NAME		"haha.wav"
#define BIT_PER_SAMPLE		16 //get_bit_per_sample(BIT_PER_SAMPLE_STR)
#define WAV_CNT_LEN			(100)

static wav_file_t *demo_wav_file;
static int demo_wav_cnt;
#endif

#ifdef PARSE_XIAOMI_DATA
#define CHANNELS_8_LEN	(1 * 8 * 32 / 8)
struct mic_data channel_8_src[FRAME_CNT];

#define unpacket()												\
	for (int i = 0; i < FRAME_CNT; i++)							\
		for (int j = 0; j < CHANNELS; j++)						\
			buf[i].amic[j] = channel_8_src[i].amic[j] >> 16;

static int find_pcm_data_flag;
void parse_xiaomi_data(mic_data_dst_t *buf, int len)
{
	if (!find_pcm_data_flag) {
		/*log_i("---------------------------1-----");*/
		int buf_len = CHANNELS_8_LEN;
		char channel_8[CHANNELS_8_LEN];
		char *tmp = channel_8;
		char *data_tmp = (char *)channel_8_src;

		mic_read_get_data(channel_8, CHANNELS_8_LEN);
		/*print_hex(channel_8, CHANNELS_8_LEN);*/

		while (buf_len-- > 0) {
			if (*tmp++ != 0x00)
				continue;
			if ((*tmp++ & 0x07) != 0)
				continue;

			break;
		}

		buf_len = tmp - channel_8 - 2;
		memcpy(data_tmp, tmp - 2, CHANNELS_8_LEN -buf_len);

		memset(channel_8, 1, CHANNELS_8_LEN);
		mic_read_get_data(channel_8, buf_len);
		/*print_hex(channel_8, buf_len);*/

		memcpy(data_tmp + CHANNELS_8_LEN - buf_len, channel_8, buf_len);
		/*print_hex(data_tmp, CHANNELS_8_LEN);*/

		mic_read_get_data(data_tmp + CHANNELS_8_LEN, sizeof(channel_8_src) - CHANNELS_8_LEN);

		find_pcm_data_flag = 1;
	} else {
		/*log_i("---------------------------2-----");*/
		mic_read_get_data(channel_8_src, sizeof(channel_8_src));

		char *tmp = (char *)channel_8_src;
		int sum = 0;
		for (int i = 0; i < FRAME_CNT; i += 32)
			sum += *(tmp + i + 1) & 0x07;
		if (sum != 0) {
			find_pcm_data_flag = 0;
			log_i("---------------------------->>> sum: %d \n", sum);
		}
	}
	unpacket();
}
#endif

int main(int argc, char *argv[])
{
	static mic_data_dst_t mic_data[FRAME_CNT];

	mic_read_init();

#ifdef SAVE_PCM
	demo_wav_file = wav_file_create(DEMO_WAV_NAME, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);
#endif

	while (1) {
#ifdef PARSE_XIAOMI_DATA
		parse_xiaomi_data(mic_data, sizeof(mic_data));
#else
		mic_read_get_data(mic_data, sizeof(mic_data));
#endif

#ifdef SAVE_PCM
		if (demo_wav_cnt++ < WAV_CNT_LEN)
			wav_file_write(demo_wav_file, mic_data, sizeof(mic_data));

		log_i("--demo_wav_cnt: %d", demo_wav_cnt);

		if (demo_wav_cnt == WAV_CNT_LEN) {
			printf("--cb--over------demo_wav_cnt: %d \n", demo_wav_cnt);
			wav_file_clean(demo_wav_file);
			break;
		}
#endif
	}

	mic_read_clean();

	return 0;
}

