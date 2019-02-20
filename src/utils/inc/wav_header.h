/**
 *
 * Release under GPLv2.
 * 
 * @file    wav_header.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    18/02 2019 11:01
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        18/02 2019      create the file
 * 
 *     last modified: 18/02 2019 11:01
 */
#ifndef __UTILS_WAV_HEADER_H_
#define __UTILS_WAV_HEADER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef U_WAV_HEADER_GB
#define U_WAV_HEADER_EX extern
#else
#define U_WAV_HEADER_EX
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define COMPOSE_ID(a,b,c,d)	((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define LE_SHORT(v)		      (v)
#define LE_INT(v)		        (v)
#define BE_SHORT(v)		      bswap_16(v)
#define BE_INT(v)		        bswap_32(v)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define COMPOSE_ID(a,b,c,d)	((d) | ((c)<<8) | ((b)<<16) | ((a)<<24))
#define LE_SHORT(v)		      bswap_16(v)
#define LE_INT(v)		        bswap_32(v)
#define BE_SHORT(v)		      (v)
#define BE_INT(v)		        (v)
#else
#error "Wrong endian"
#endif

#define WAV_RIFF		COMPOSE_ID('R','I','F','F')
#define WAV_WAVE		COMPOSE_ID('W','A','V','E')
#define WAV_FMT			COMPOSE_ID('f','m','t',' ')
#define WAV_DATA		COMPOSE_ID('d','a','t','a')

/* WAVE fmt block constants from Microsoft mmreg.h header */
#define WAV_FMT_PCM             0x0001
#define WAV_FMT_IEEE_FLOAT      0x0003
#define WAV_FMT_DOLBY_AC3_SPDIF 0x0092
#define WAV_FMT_EXTENSIBLE      0xfffe

typedef struct _riff_tag {
    uint32_t magic;     //"RIFF"
    uint32_t len;	    // RIFF chunk data size,exclude riff_id[4] and riff_datasize,total - 8
    uint32_t type;      // "WAVE"
} wav_riff_t;

typedef struct _fmt_tag {
    uint32_t magic;         // "fmt "
    uint32_t size;          // fmt chunk data size,16 for pcm
    uint16_t format;        // 1 for PCM
    uint16_t channel;       // 1(mono) or 2(stereo)
    uint32_t sample_rate;   // samples per second
    uint32_t byte_rate;     // sample_rate * channels * bit_per_sample / 8
    uint16_t block_align;   // number bytes per sample, bit_per_sample * channels / 8
    uint16_t sample_length; // bits of each sample(8,16,32).
} wav_fmt_t;

typedef struct _data_tag {
    uint32_t magic;     // "data"
    uint32_t length;    // data chunk size,pcm_size - 44
} wav_data_t;

typedef struct _wav_header_tag {
    wav_riff_t riff;
    wav_fmt_t  fmt;
    wav_data_t data;
} wav_header_t;

#define WAV_HEADER_RIFF_LEN sizeof(wav_riff_t)
#define WAV_HEADER_FMT_LEN  sizeof(wav_fmt_t)
#define WAV_HEADER_DATA_LEN sizeof(wav_data_t)

U_WAV_HEADER_EX int wav_header_r(int fd, wav_header_t * wav_header);
U_WAV_HEADER_EX int wav_header_w(int fd, wav_header_t * wav_header,
    uint32_t channel, uint32_t sample_rate, uint32_t sample_length);

U_WAV_HEADER_EX void wav_update_header(wav_header_t *wav_header, uint32_t pcm_len);

#ifdef __cplusplus
}
#endif

#endif

