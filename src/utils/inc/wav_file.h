/**
 *
 * Release under GPLv2.
 * 
 * @file    wav_file.h
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
#ifndef __UTILS_WAV_FILE_H_
#define __UTILS_WAV_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "wav_header.h"

#ifndef U_WAV_FILE_GB
#define U_WAV_FILE_EX extern
#else
#define U_WAV_FILE_EX
#endif

typedef struct _wav_file_tag {
    wav_header_t wav_header;
    int fd;
    int open_create_flag;
    uint32_t pcm_len;
} wav_file_t;

U_WAV_FILE_EX int wav_file_open(const char * const name, wav_file_t * const wav_file);
U_WAV_FILE_EX int wav_file_create(const char * const name, wav_file_t * const wav_file,
    uint32_t channel, uint32_t sample_rate, uint32_t sample_length);
U_WAV_FILE_EX int wav_file_close(wav_file_t * const wav_file);

U_WAV_FILE_EX int wav_file_read(const wav_file_t * const wav_file, char *buf, int len);
U_WAV_FILE_EX int wav_file_write(wav_file_t * const wav_file, char *buf, int len);

#ifdef __cplusplus
}
#endif

#endif

