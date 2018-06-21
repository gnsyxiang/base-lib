/**
 * 
 * Release under GPLv2.
 * 
 * @file    vad-cutting.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/03 2018 16:29
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/03 2018      create the file
 * 
 *     last modified: 05/03 2018 16:29
 */
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include "wav_helper.h"
#include "signal_helper.h"
#include "cnt_vad.h"
#include "log_helper.h"

#define DATA_POINT_LEN	(320)
#define FRAME_LEN_MS	(20)

#define FRAME_COUNT		(10) // 连续语音时间，(FRAME_COUNT * FRAME_LEN_MS)
#define SAVE_FRONT_CNT	(5)	 // 多保存前节点的长度，(SAVE_FRONT_CNT * FRAME_LEN_MS)
#define SAVE_TAIL_CNT	(5)	 // 多保存前节点的长度，(SAVE_TAIL_CNT * FRAME_LEN_MS)
#define VOICE_MID_CNT	(22) // 语音中，词与词之间的间隔时间，(VOICE_MID_CNT * FRAME_LEN_MS)

#define WAV_DIR					"wav/"
#define VAD_CUTTING_WAV_PATH	WAV_DIR"test.wav"
#define VAD_SAVE_WAV_PATH		WAV_DIR"dst"


int vad_test(int (*voice_time)[2])
{
	short buffer[DATA_POINT_LEN] = {0};
	int frame_cnt = 0;
	int vad_flag = 0, front_frame_cnt = 0;
    int voice_flag = 0;
    int voice_flag_cnt = 0;
    int wav_num = 0;
	int quit_cnt = 0;
	short ret;
	cntVadState vad_stat;

	cntVAD16kInit(&vad_stat);

	log_i("--------3");
	wav_file_t *wav_file = wav_file_open("./vad-test.wav");
	log_i("--------4");

	while (1) {
		if (wav_file_read(wav_file, buffer, DATA_POINT_LEN * 2) <= 0)
			break;

		ret = cntVAD16k(buffer,&vad_stat);
        if (1 == ret) {
			log_i("time: %d.%d", frame_cnt * FRAME_LEN_MS / 1000, frame_cnt * FRAME_LEN_MS % 1000);
			voice_flag_cnt = 0;
            if (front_frame_cnt++ >= FRAME_COUNT) {
				vad_flag = 1;
			}
        } else {
			if (quit_cnt++ > 3) {
				quit_cnt = 0;
				front_frame_cnt = 0;
				vad_flag = 0;
			}
        }

		if(vad_flag == 1 && voice_flag == 0) {
			voice_flag = 1;
			voice_time[wav_num][0] = frame_cnt - FRAME_COUNT - SAVE_FRONT_CNT;
			/*log_i("start: %d.%d", voice_time[wav_num][0] * FRAME_LEN_MS / 1000, voice_time[wav_num][0] * FRAME_LEN_MS % 1000);*/
        } else if (voice_flag == 1 && voice_flag_cnt++ == VOICE_MID_CNT) {
			voice_flag = 0;

			voice_time[wav_num][1] = frame_cnt - VOICE_MID_CNT + SAVE_TAIL_CNT;
			wav_num++;
        }

		frame_cnt++;
	}

	wav_file_clean(wav_file);

	return wav_num;
}

#if 0

#define CHANNELS		(1)
#define SAMPLE_RATE		(16000)
#define BIT_PER_SAMPLE	(16)

#define BYTES_OF_1S		(1 * SAMPLE_RATE * BIT_PER_SAMPLE / 8)
#define BYTES_OF_1MS	(BYTES_OF_1S / 1000)

#define WAV_S_LEN		(15)

void save_wav(int (*voice_time)[2], int wav_num)
{
    char path[256] = {0};
    static char voice_buf[WAV_S_LEN * BYTES_OF_1S] = {0};

	wav_file_t *wav_test_file = wav_file_open(VAD_CUTTING_WAV_PATH);

    if (access(VAD_SAVE_WAV_PATH, F_OK) != 0) {
        mkdir(VAD_SAVE_WAV_PATH, S_IRWXU);
    }

    for (int i = 0; i < wav_num; i++) {
		if (i >= 785)
			sprintf(path, "%s/%04d.wav", VAD_SAVE_WAV_PATH, i+1);
		else
			sprintf(path, "%s/%04d.wav", VAD_SAVE_WAV_PATH, i);

		wav_file_t *wav_file = wav_file_create(path, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

        int voice_bytes = voice_time[i][0] * FRAME_LEN_MS * BYTES_OF_1MS;
        int voice_last_bytes = (voice_time[i][1] - voice_time[i][0]) * FRAME_LEN_MS * BYTES_OF_1MS;

		wav_file_seek(wav_test_file, voice_bytes, SEEK_SET);
		wav_file_read(wav_test_file, voice_buf, voice_last_bytes);

		wav_file_write(wav_file, voice_buf, voice_last_bytes);
		wav_file_clean(wav_file);

		log_i("start: %d.%d", voice_time[i][0] * FRAME_LEN_MS / 1000, voice_time[i][0] * FRAME_LEN_MS % 1000);
		log_i("-----------end: %d.%d", voice_time[i][1] * FRAME_LEN_MS / 1000, voice_time[i][1] * FRAME_LEN_MS % 1000);
		printf("\n");
    }

	log_i("wav_num: %d", wav_num);

	wav_file_clean(wav_test_file);
}
#endif

#define FRAME_POINT_CNT		(320)
#define FRAME_TO_MS			(20)

#define FRAME_CONTINUE_START_CNT	(5)			// 连续语音时间，(FRAME_COUNT * FRAME_TO_MS)
#define FRAME_CONTINUE_STOP_CNT		(3)			// 尾断点连续语音时间
#define MID_BLANK_TIME_CNT			(22)		// 中间音频空白时间

#define cur_time_s(frame_cnt)	((frame_cnt) * FRAME_TO_MS / 1000)
#define cur_time_ms(frame_cnt)	((frame_cnt) * FRAME_TO_MS % 1000)

void vad_handle(const char *src_wav_name)
{
	int voice_time[2000][2] = {0};

	short ret;
	cntVadState vad_stat;
	cntVAD16kInit(&vad_stat);

	wav_file_t *wav_file = wav_file_open(src_wav_name);

	short buf[FRAME_POINT_CNT] = {0};
	int frame_cnt = 0;					// 处理音频的帧数
	int is_a_voice_flag = 0;
	int mid_time_cnt = 0;				// 语音中间停顿计数
	int voice_cnt = 0;

	int front_time_cnt = 0;				// 连续语音计数，方可判断语音开始
	int is_voice_start_flag = 0;		// 判断语音是否开始

	int back_time_cnt = 0;				// 连续语音计数，方可判断语音结束
	int is_voice_stop_flag = 0;			// 判断语音是否结束

	while (1) {
		if (wav_file_read(wav_file, buf, FRAME_POINT_CNT * sizeof(short)) <= 0)
			break;

		ret = cntVAD16k(buf, &vad_stat);
		if (ret) {
			/*log_i("-----1----");*/
			if (front_time_cnt++ == FRAME_CONTINUE_START_CNT) {
			/*log_i("-----2----");*/
				is_voice_stop_flag = 0;
				is_voice_start_flag = 1;
				back_time_cnt = 0;
			}
		} else {
			/*log_i("-----3----");*/
			if (is_voice_start_flag && back_time_cnt++ == FRAME_CONTINUE_STOP_CNT) {
			/*log_i("-----4----");*/
					is_voice_start_flag = 0;
					front_time_cnt = 0;
					is_voice_stop_flag = 1;

					mid_time_cnt = 0;
			}
		}

		if (is_voice_start_flag && !is_a_voice_flag) {
			is_a_voice_flag = 1;
			printf("time: %d.%03d \t", 
					cur_time_s(frame_cnt - FRAME_CONTINUE_START_CNT), 
					cur_time_ms(frame_cnt - FRAME_CONTINUE_START_CNT));
		} else if (is_a_voice_flag && is_voice_stop_flag && mid_time_cnt++ == MID_BLANK_TIME_CNT) {
			is_a_voice_flag = 0;
			printf("%d.%03d \n", 
					cur_time_s(frame_cnt - FRAME_CONTINUE_STOP_CNT - MID_BLANK_TIME_CNT), 
					cur_time_ms(frame_cnt - FRAME_CONTINUE_STOP_CNT - MID_BLANK_TIME_CNT));
			voice_cnt++;
		}

		frame_cnt++;
	}

	log_i("voice_cnt: %d", voice_cnt);

	wav_file_clean(wav_file);
}

static void dis_help_info(const char *name)
{
	printf("vad cutting wav \n");
	printf("\t -s	src dir \n");
	printf("\t -h	help info \n");
	printf("eg:  \n");
	printf("\t ./main -s wav/src \n");
}

#include <string.h>
#define WAV_NAME_LEN	(128)

int main(int argc, char** argv)
{
	char src_wav_name[WAV_NAME_LEN];

	if (argc < 2) {
		dis_help_info(argv[0]);
		exit(0);
	}

	int opt;
	while (-1 != (opt = getopt(argc, argv, "s:d:f:b:h"))) {
		switch (opt) {
			case 's':
				memset(src_wav_name, '\0', WAV_NAME_LEN);
				strcpy(src_wav_name, optarg);
				break;
			case 'h':
			default:
				dis_help_info(argv[0]);
				exit(0);
		}
	}

	register_linux_signal_hanler(argv[0]);

	vad_handle(src_wav_name);

	return 0;
}

