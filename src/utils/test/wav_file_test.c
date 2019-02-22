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
#include <stdlib.h>
#include <string.h>

#include "parse_cmd.h"
#include "wav_header.h"
#include "wav_file.h"
#include "log_helper.h"
#include "dir_helper.h"

#define SAMPLE_NUM      (1024)

#define CHANNEL         (5)
#define SAMPLE_RATE     (16000)
#define SAMPLE_LENGTH   (16)

#define FRONT_MS        (3000)
#define TAIL_MS         (2000)
#define BYTES_1MS       (SAMPLE_RATE / 1000 * SAMPLE_LENGTH / 8 * CHANNEL) * 1UL
#define FRONT_MS_BYTE   (FRONT_MS * BYTES_1MS) * 1UL
#define TAIL_MS_BYTE    (TAIL_MS  * BYTES_1MS) * 1UL

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

static void add_blank_time(wav_file_t * wav_file, int len)
{
    char *buf = malloc(len);
    if (!buf) {
        log_e("faild");
    }
    memset(buf, '\0', len);
    wav_file_write(wav_file, buf, len);
    free(buf);
}

static void create_new_file(const char * const src, char *dst, int time_ms)
{
    wav_file_t wav_file_r;
    wav_file_t wav_file_w;
    char buf[SAMPLE_NUM];
    int ret;
    unsigned long total_bytes = time_ms * BYTES_1MS;
    unsigned long sum_ret = 0;

    wav_file_open(src, &wav_file_r);
    wav_file_create(dst, &wav_file_w, CHANNEL, SAMPLE_RATE, SAMPLE_LENGTH);

    add_blank_time(&wav_file_w, FRONT_MS_BYTE);

    while ((ret = wav_file_read(&wav_file_r, buf, SAMPLE_NUM)) > 0) {
        wav_file_write(&wav_file_w, buf, ret);
        sum_ret += ret;
    }

    if (time_ms) {
        int len = total_bytes - FRONT_MS_BYTE - sum_ret;
        add_blank_time(&wav_file_w, len);
    } else {
        add_blank_time(&wav_file_w, TAIL_MS_BYTE);
    }

    wav_file_close(&wav_file_w);
    wav_file_close(&wav_file_r);
}

static void handle_wav_cb(const char * const name, unsigned char d_type, void *args)
{
    log_i("name: %s", name);

    char buf[32] = {0};
    sprintf(buf, "wav/dst/%s", name);

    create_new_file(name, buf, 50000);
}

static void stretch_audio(void)
{
    read_dir_file("wav/src", handle_wav_cb, NULL);
}

static void wav_file_test_init(void)
{
	printf("20. wav_file test \n");
	printf("21. stretch audio test \n");

	handle_test_cmd_t wav_file_test_cmd[] = {
		{"20", wav_file_test},
		{"21", stretch_audio},
	};

	register_test_cmd(wav_file_test_cmd, ARRAY_NUM(wav_file_test_cmd));
}
DECLARE_INIT(wav_file_test_init);

