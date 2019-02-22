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


/**
 * @brief wav_file结构体
 */
typedef struct _wav_file_tag {
    int fd;                     // 打开wav的句柄
    int pcm_len;                // pcm数据长度
    int open_create_flag;       // 区分打开和新建wav文件，为新建文件更新wav头准备

    wav_header_t wav_header;    // wav头结构体
} wav_file_t;

/**
 * @brief 打开wav文件
 *
 * @param name: wav文件名
 * @param wav_file: wav_file结构体指针
 *
 * @return 成功返回0，失败为负值
 */
U_WAV_FILE_EX int wav_file_open(const char * const name, wav_file_t * const wav_file);

/**
 * @brief 新建wav文件
 *
 * @param name: wav文件名
 * @param wav_file: wav_file结构体指针
 * @param channel: 音频包含的通道号
 * @param sample_rate: 采样频率
 * @param sample_length: 采样精度
 *
 * @return 成功返回0，失败为负值
 */
U_WAV_FILE_EX int wav_file_create(const char * const name, wav_file_t * const wav_file,
    uint32_t channel, uint32_t sample_rate, uint32_t sample_length);

/**
 * @brief 关闭wav文件
 *
 * @param wav_file: wav_file结构体指针
 *
 * @return 成功返回0，失败为负值
 */
U_WAV_FILE_EX int wav_file_close(wav_file_t * const wav_file);

/**
 * @brief 读取pcm数据
 *
 * @param wav_file: wav_file结构体指针
 * @param buf: 读取pcm数据的指针
 * @param len: 读取pcm的长度
 *
 * @return 成功返回读取的长度，失败返回-1
 */
U_WAV_FILE_EX int wav_file_read(const wav_file_t * const wav_file, char * const buf, int len);

/**
 * @brief 
 *
 * @param wav_file
 * @param buf: 写入pcm数据的指针
 * @param len: 写入pcm的长度
 *
 * @return 成功返回写入的长度，失败返回-1
 */
U_WAV_FILE_EX int wav_file_write(wav_file_t * const wav_file, const char * const buf, int len);

#ifdef __cplusplus
}
#endif

#endif

