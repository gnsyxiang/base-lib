/****************************************************************
 * Filename: test_wav.c
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 23/11 2017 17:49
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	23/11 2017
 ****************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "wav.h"
#include "wav_helper.h"

#define NEW_WAV_PATH "new_wav"

#define ALIGN4(size) (((size) + 3) & ~(0x3))
#define ALIGN3(size) (((size) + 2) & ~(0x2))
#define ALIGN2(size) (((size) + 1) & ~(0x1))

void wav_handle(const char *base_path, const char *name)
{
    wav_t *wav = NULL;
    struct wav_writer *wav_tmp;

    char src_name[256] = {0};
    char dst_name[256] = {0};
    char dst_dir[256] = {0};

    sprintf(src_name, "%s/%s", base_path, name);
    sprintf(dst_dir, "%s/%s", base_path, NEW_WAV_PATH);
    sprintf(dst_name, "%s/%s", dst_dir, name);

    if (access(dst_dir, F_OK) != 0) {
        mkdir(dst_dir, S_IRWXU);
    }

    wav = wav_open(src_name);
    wav_tmp = create_wav_writer_by_path(dst_name, 1, 16000, 16);

    int total_bytes;
    total_bytes = 15 * 16000 * 2;

    int bps = wav->format.bits_per_sample / 8;
    int blank_bytes;
    blank_bytes = (total_bytes - wav->data.size) / 2;
    if (bps == 2)
        blank_bytes = ALIGN2(blank_bytes);
    else if (bps == 3)
        blank_bytes = ALIGN3(blank_bytes);
    else if (bps == 4)
        blank_bytes = ALIGN4(blank_bytes);

    char buf[blank_bytes];
    char voice[wav->data.size];

    memset(buf, '\0', blank_bytes);

    wav_writer_write(wav_tmp, buf, blank_bytes);
    /* printf("blank_bytes: %d \n", blank_bytes); */

    /* wav_dump(wav); */

    /* printf("offset: %d \n", wav->data_offset); */
    /* printf("size: %d \n", wav->data.size); */

	fseek(wav->fp, wav->data_offset, SEEK_SET);
    wav_read_data(wav, voice, wav->data.size);
    wav_writer_write(wav_tmp, voice, wav->data.size);

    wav_writer_write(wav_tmp, buf, blank_bytes);

    wav_writer_flush(wav_tmp);
    delete_wav_writer_and_file_descriptor(wav_tmp);
    wav_close(&wav);
}
