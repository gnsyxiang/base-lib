/**
 * 
 * Release under GPLv2.
 * 
 * @file    wav_file_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/02 2019 15:31
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/02 2019      create the file
 * 
 *     last modified: 20/02 2019 15:31
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "parse_cmd.h"
#include "wav_header.h"
#include "wav_file.h"
#include "log_helper.h"

#define SAMPLE_NUM      (1024)

#define CHANNEL         (5)
#define SAMPLE_RATE     (16000)
#define SAMPLE_LENGTH   (16)

static void wav_file_test(void)
{
    wav_file_t wav_file_r;
    wav_file_t wav_file_w;
    char buf[SAMPLE_NUM];
    int ret;

    wav_file_open("haha.wav", &wav_file_r);
    wav_file_create("xixi.wav", &wav_file_w, CHANNEL, SAMPLE_RATE, SAMPLE_LENGTH);

    while ((ret = wav_file_read(&wav_file_r, buf, SAMPLE_NUM)) > 0) {
        wav_file_write(&wav_file_w, buf, ret);
    }

    wav_file_close(&wav_file_w);
    wav_file_close(&wav_file_r);
}

static void wav_file_test_init(void)
{
	printf(" 2. wav_file test \n");

	handle_test_cmd_t wav_file_test_cmd[] = {
		{"2", wav_file_test},
	};

	register_test_cmd(wav_file_test_cmd, ARRAY_NUM(wav_file_test_cmd));
}
DECLARE_INIT(wav_file_test_init);

