/**
 * 
 * Release under GPLv2.
 * 
 * @file    wav_header.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    18/02 2019 11:36
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        18/02 2019      create the file
 * 
 *     last modified: 18/02 2019 11:36
 */
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define U_WAV_HEADER_GB
#include "wav_header.h"
#undef U_WAV_HEADER_GB

// #define WAV_P_HEADER

#ifdef WAV_P_HEADER
static char *format_to_str(uint16_t fmt)
{
    switch (fmt) {
        case WAV_FMT_PCM:
            return "PCM";
            break;
        case WAV_FMT_IEEE_FLOAT:
            return "IEEE FLOAT";
            break;
        case WAV_FMT_DOLBY_AC3_SPDIF:
            return "DOLBY AC3 SPDIF";
            break;
        case WAV_FMT_EXTENSIBLE:
            return "EXTENSIBLE";
            break;
        default:
            break;
	}

	return "NON Support Fmt";
}

static void print_wav_header(wav_header_t *wav_header)
{
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("\n");
	
	printf("file riff magic:    [%c%c%c%c]\n", 
		(char)(wav_header->riff.magic), 
		(char)(wav_header->riff.magic >> 8), 
		(char)(wav_header->riff.magic >> 16), 
		(char)(wav_header->riff.magic >> 24));
	printf("file length:        [%d]\n", wav_header->riff.len);
	printf("file Type:          [%c%c%c%c]\n",
		(char)(wav_header->riff.type), 
		(char)(wav_header->riff.type >> 8), 
		(char)(wav_header->riff.type >> 16), 
		(char)(wav_header->riff.type >> 24));
		
	printf("\n");

	printf("fmt magic:          [%c%c%c%c]\n",
		(char)(wav_header->fmt.magic), 
		(char)(wav_header->fmt.magic >> 8), 
		(char)(wav_header->fmt.magic >> 16), 
		(char)(wav_header->fmt.magic >> 24));
	printf("fmt size:           [%d]\n", wav_header->fmt.size);
	printf("fmt format:         [%s]\n", format_to_str(wav_header->fmt.format));
	printf("fmt channel:        [%d]\n", wav_header->fmt.channel);
	printf("fmt sample_rate:    [%d](Hz)\n", wav_header->fmt.sample_rate);
	printf("fmt byte_rate:      [%d](B/s)\n", wav_header->fmt.byte_rate);
	printf("fmt block_align:    [%d]\n", wav_header->fmt.block_align);
	printf("fmt sample_length:  [%d]\n", wav_header->fmt.sample_length);
	
	printf("\n");

	printf("data magic:         [%c%c%c%c]\n",
		(char)(wav_header->data.magic), 
		(char)(wav_header->data.magic >> 8), 
		(char)(wav_header->data.magic >> 16), 
		(char)(wav_header->data.magic >> 24));
	printf("data length:        [%d]\n", wav_header->data.length);
	
	printf("\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}
#endif

static int check_wav_header_valid(wav_header_t *wav_header)
{
	if (wav_header->riff.magic != WAV_RIFF
            || wav_header->riff.type != WAV_WAVE
            || wav_header->fmt.magic != WAV_FMT
            || wav_header->fmt.size != LE_INT(16)
            || (wav_header->fmt.channel != LE_SHORT(1)
                && wav_header->fmt.channel != LE_SHORT(2)
                && wav_header->fmt.channel != LE_SHORT(3)
                && wav_header->fmt.channel != LE_SHORT(4)
                && wav_header->fmt.channel != LE_SHORT(5))
            || wav_header->data.magic != WAV_DATA) {
		
		fprintf(stderr, "non standard wav file.\n");
		return -1;
	}

	return 0;
}

int wav_header_r(int fd, wav_header_t * wav_header)
{
	assert((fd >=0) && wav_header);

	if (read(fd, &wav_header->riff, WAV_HEADER_RIFF_LEN) != WAV_HEADER_RIFF_LEN
     || read(fd, &wav_header->fmt,  WAV_HEADER_FMT_LEN)  != WAV_HEADER_FMT_LEN
     || read(fd, &wav_header->data, WAV_HEADER_DATA_LEN) != WAV_HEADER_DATA_LEN) {

		fprintf(stderr, "Error WAV_ReadHeader\n");
		return -1;
	}

    if (check_wav_header_valid(wav_header) < 0)
        return -2;

#ifdef WAV_P_HEADER
    print_wav_header(wav_header);
#endif

    return 0;
}

#define FORMAT_PCM	(1)			// 表示存储pcm数据
static void wav_create_header(wav_header_t *wav_header, 
        uint32_t channel, uint32_t sample_rate, uint32_t sample_length)
{
	wav_header->riff.magic = WAV_RIFF;
    wav_header->riff.len   = 0;
    wav_header->riff.type  = WAV_WAVE;

    wav_header->fmt.magic         = WAV_FMT;
    wav_header->fmt.size          = LE_INT(16);
    wav_header->fmt.format        = FORMAT_PCM;
    wav_header->fmt.channel       = LE_SHORT(channel);
    wav_header->fmt.sample_rate   = sample_rate;
    wav_header->fmt.byte_rate     = (channel * sample_length / 8) * sample_rate;
    wav_header->fmt.block_align   = channel * sample_length / 8;
    wav_header->fmt.sample_length = sample_length;

    wav_header->data.magic  = WAV_DATA;
    wav_header->data.length = 0;
}

void wav_update_header(wav_header_t *wav_header,  uint32_t pcm_len)
{
	assert((pcm_len > 0) && wav_header);

    wav_header->data.length = pcm_len + 44;
    wav_header->riff.len    = pcm_len + 44 - 8;
}

int wav_header_w(int fd, wav_header_t * wav_header,
    uint32_t channel, uint32_t sample_rate, uint32_t sample_length)
{
	assert((fd >=0) && wav_header);

    wav_create_header(wav_header, channel, sample_rate, sample_length);
	
	if (check_wav_header_valid(wav_header) < 0)
		return -1;

	if (write(fd, &wav_header->riff, WAV_HEADER_RIFF_LEN) != WAV_HEADER_RIFF_LEN
     || write(fd, &wav_header->fmt,  WAV_HEADER_FMT_LEN)  != WAV_HEADER_FMT_LEN
     || write(fd, &wav_header->data, WAV_HEADER_DATA_LEN) != WAV_HEADER_DATA_LEN) {
		
		fprintf(stderr, "Error WAV_WriteHeader\n");
		return -2;
	}

#ifdef WAV_P_HEADER
    print_wav_header(wav_header);
#endif

    return 0;
}

