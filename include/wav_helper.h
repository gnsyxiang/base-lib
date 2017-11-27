#ifndef WAV_HELPER_H_
#define WAV_HELPER_H_

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

struct wav_writer;

struct wav_writer *create_wav_writer_by_file(FILE *file, int channels,
		int sample_rate, int bits_per_sample);

void delete_wav_writer(struct wav_writer *writer);

struct wav_writer *create_wav_writer_by_path(const char *path, int channels,
		int sample_rate, int bits_per_sample);

void delete_wav_writer_and_file_descriptor(struct wav_writer *writer);

int wav_writer_write(struct wav_writer *writer, void *data, int len);

void wav_writer_flush(struct wav_writer *writer);

#ifdef  __cplusplus
}
#endif

#endif /* WAV_HELPER_H_ */
