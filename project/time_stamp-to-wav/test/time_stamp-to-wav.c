/**
 * 
 * Release under GPLv2.
 * 
 * @file    time_stamp-to-wav.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/06 2018 14:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/06 2018      create the file
 * 
 *     last modified: 21/06 2018 14:16
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <log_helper.h>
#include <wav_helper.h>
#include <typedef_helper.h>

#define WAV_NAME_LEN	(128)

#define channels(wav_file)			wav_file->fmt->fmt_channels
#define sample_rate(wav_file)		wav_file->fmt->fmt_sample_rate
#define bit_per_sample(wav_file)	wav_file->fmt->fmt_bits_per_sample
#define data_len(wav_file)			wav_file->data->data_sz

static int calc_blank_bytes(wav_file_t *wav_file, float time_s)
{
	int blank_bytes = time_s * 
		channels(wav_file) * sample_rate(wav_file) * bit_per_sample(wav_file) / 8;

	switch (bit_per_sample(wav_file) / 8) {
		case 2: blank_bytes = ALIGN2(blank_bytes); break;
		case 3: blank_bytes = ALIGN3(blank_bytes); break;
		case 4: blank_bytes = ALIGN4(blank_bytes); break;

		default: log_i("bsp is error"); break;
	}

	return blank_bytes;
}

#define CHANNELS		(1)
#define SAMPLE_RATE		(16000)
#define BIT_PER_SAMPLE	(16)

void time_stamp_to_wav(const char *src_name, const char *file_name, 
		float start_time, float stop_time)
{
	log_i("start_time: %f", start_time);
	log_i("stop_time: %f \n", stop_time);

	wav_file_t *new_wav_file = wav_file_create(file_name, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);
	wav_file_t *test_wav_file = wav_file_open(src_name);

	int start_byte = calc_blank_bytes(test_wav_file, start_time);
	int copy_bytes = calc_blank_bytes(test_wav_file, stop_time - start_time);

	wav_file_seek(test_wav_file, start_byte, SEEK_SET);

	short buf[1024];
	int ret;
	int read_len = sizeof(buf);
	while (copy_bytes) {
		ret = wav_file_read(test_wav_file, buf, read_len);
		wav_file_write(new_wav_file, buf, ret);

		copy_bytes -= ret;
		if (copy_bytes > sizeof(buf))
			read_len = sizeof(buf);
		else
			read_len = copy_bytes;
	}

	wav_file_clean(new_wav_file);
	wav_file_clean(test_wav_file);
}

static void dis_help_info(const char *name)
{
	printf("time stamp to wav \n");
	printf("\t -t	the src file name \n");
	printf("\t -f	the new file name \n");
	printf("\t -s	start time stamp \n");
	printf("\t -p	stop time stamp \n");
	printf("\t -h	help info \n");
	printf("eg:  \n");
	printf("\t ./main -t src.wav -f name.wav -s 2 -p 2 \n");
}

int main(int argc, char **argv)
{
	char file_name[WAV_NAME_LEN];
	char src_name[WAV_NAME_LEN];
	float start_time;
	float stop_time;

	if (argc < 9) {
		dis_help_info(argv[0]);
		exit(0);
	}

	int opt;
	while (-1 != (opt = getopt(argc, argv, "f:s:p:t:h"))) {
		switch (opt) {
			case 't':
				memset(src_name, '\0', WAV_NAME_LEN);
				strcpy(src_name, optarg);
				break;
			case 'f':
				memset(file_name, '\0', WAV_NAME_LEN);
				strcpy(file_name, optarg);
				break;
			case 's':
				start_time = atof(optarg);
				break;
			case 'p':
				stop_time = atof(optarg);
				break;
			case 'h':
			default:
				dis_help_info(argv[0]);
				exit(0);
		}
	}

	time_stamp_to_wav(src_name, file_name, start_time, stop_time);

	return 0;
}

