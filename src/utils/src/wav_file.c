/**
 * 
 * Release under GPLv2.
 * 
 * @file    wav_file.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/02 2019 17:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/02 2019      create the file
 * 
 *     last modified: 20/02 2019 17:08
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "log_helper.h"
#include "wav_header.h"

#define U_WAV_FILE_GB
#include "wav_file.h"
#undef U_WAV_FILE_GB

int wav_file_open(const char * const name, wav_file_t * const wav_file)
{
    assert(name && wav_file);

    wav_file->open_create_flag = 1;

    wav_file->fd = open(name, O_RDONLY);
    if (wav_file->fd == -1) {
        log_e("open faild");
        return -1;
    }

    if (wav_header_r(wav_file->fd, &wav_file->wav_header) != 0) {
        log_e("wav_header_r faild");
        return -2;
    }

    return 0;
}

int wav_file_create(const char * const name, wav_file_t * const wav_file,
    uint32_t channel, uint32_t sample_rate, uint32_t sample_length)
{
    assert(name && wav_file 
            && channel > 0 && sample_rate > 0 && sample_length > 0);

    wav_file->open_create_flag = 2;
    wav_file->pcm_len = 0;

    wav_file->fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0664);
    if (wav_file->fd < 0) {
        log_e("open faild");
        return -1;
    }

    if (wav_header_w(wav_file->fd, &wav_file->wav_header,
                channel, sample_rate, sample_length) != 0) {
        log_e("wav_header_w faild");
        return -2;
    }

    return 0;
}

int wav_file_close(wav_file_t * const wav_file)
{
    assert(wav_file);

    if (wav_file->open_create_flag == 2) {
        wav_update_header(&wav_file->wav_header, wav_file->pcm_len);
        lseek(wav_file->fd, SEEK_SET, 0);
        write(wav_file->fd, &wav_file->wav_header, sizeof(wav_file->wav_header));
    }

    close(wav_file->fd);

    return 0;
}

int wav_file_read(const wav_file_t * const wav_file, char *buf, int len)
{
    assert(wav_file && buf && len > 0);

    return read(wav_file->fd, buf, len);
}

int wav_file_write(wav_file_t * const wav_file, char *buf, int len)
{
    assert(wav_file && buf && len > 0);

    wav_file->pcm_len += len;
    
    return write(wav_file->fd, buf, len);
}

