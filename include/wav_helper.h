/****************************************************************
 * Filename: wav_helper.h
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 26/11 2017 21:47
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	26/11 2017
 ****************************************************************/
#ifndef _WAV_HELPER_H
#define _WAV_HELPER_H

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

    typedef struct _wav_header_tag {
        char  riff_id[4];               //"RIFF"
        int   riff_size;                // RIFF chunk data size,exclude riff_id[4] and riff_datasize,total - 8
        char  riff_format[4];           // "WAVE"

        char  fmt_id[4];                // "fmt "
        int   fmt_size;                 // fmt chunk data size,16 for pcm
        short fmt_audio_format;         // 1 for PCM
        short fmt_channels;             // 1(mono) or 2(stereo)
        int   fmt_sample_rate;          // samples per second
        int   fmt_byte_rate;            // sample_rate * channels * bit_per_sample / 8
        short fmt_block_align;          // number bytes per sample, bit_per_sample * channels / 8
        short fmt_bit_per_sample;       // bits of each sample(8,16,32).

        char  data_id[4];               // "data"
        int   data_size;                // data chunk size,pcm_size - 44
    }wav_header_t;

    typedef struct _wav_writer_tag {
        struct wav_header_t wav_header; // wav header
        int    play_ms;                 // data_size / fmt_byte_rate
        FILE   *file;
    }wav_writer_t;

    wav_writer_t *wav_writer_init(const char *path, int channels, int sample_rate, int bit_per_sample);
    void wav_writer_clean(wav_writer_t *wav_writer);

    void wav_header_write(wav_writer_t *wav_writer);
    void wav_header_read(wav_writer_t *wav_writer);
    void wav_header_dump(wav_header_t *wav_writer);

    void wav_writer_write(wav_writer_t *wav_writer, void *data, int len);
    void wav_writer_read(wav_writer_t *wav_writer, void *data, int len);

    void wav_writer_flust(wav_writer_t *wav_writer);
    void wav_writer_rewind(wav_writer_t *wav_writer);
    int wav_writer_over(wav_writer_t *wav_writer);

#ifdef  __cplusplus
}
#endif

#endif /* _WAV_HELPER_H */
