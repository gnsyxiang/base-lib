#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "wav_helper.h"

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

struct wav_header {
	uint32_t riff_id;
	uint32_t riff_sz;
	uint32_t riff_fmt;
	uint32_t fmt_id;
	uint32_t fmt_sz;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;
	uint32_t data_id;
	uint32_t data_sz;
};

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1


struct wav_writer *create_wav_writer_by_file(FILE *file, int channels,
		int sample_rate, int bits_per_sample) {
	struct wav_writer *writer;

	writer = malloc(sizeof(*writer));

	writer->wav.riff_id = ID_RIFF;
	writer->wav.riff_sz = 0;
	writer->wav.riff_fmt = ID_WAVE;
	writer->wav.fmt_id = ID_FMT;
	writer->wav.fmt_sz = 16;
	writer->wav.audio_format = FORMAT_PCM;
	writer->wav.num_channels = channels;
	writer->wav.sample_rate = sample_rate;
	writer->wav.bits_per_sample = bits_per_sample;
	writer->wav.byte_rate = (channels * bits_per_sample / 8) * sample_rate;
	writer->wav.block_align = channels * bits_per_sample / 8;
	writer->wav.data_id = ID_DATA;
	writer->wav.data_sz = 0;

	writer->file = file;

	fseek(writer->file, sizeof(writer->wav), SEEK_SET);

	return writer;
}

struct wav_writer *create_wav_writer_by_path(const char *path, int channels,
		int sample_rate, int bits_per_sample) {
	FILE *file = fopen(path, "w+");
	assert(file);

	return create_wav_writer_by_file(file, channels, sample_rate,
			bits_per_sample);
}

int wav_writer_write(struct wav_writer *writer, void *data, int len) {
	int ret;
	ret = fwrite(data, 1, len, writer->file);
	if (ret > 0)
		writer->wav.data_sz += ret;
	fseek(writer->file, 0, SEEK_SET);
	fwrite(&writer->wav, 1, sizeof(writer->wav), writer->file);
	fseek(writer->file, 0, SEEK_END);
	return ret;
}

void wav_writer_flush(struct wav_writer *writer) {
	fseek(writer->file, 0, SEEK_SET);
	fwrite(&writer->wav, 1, sizeof(writer->wav), writer->file);
	fflush(writer->file);
}

void delete_wav_writer_and_file_descriptor(struct wav_writer *writer) {
	/* wav_writer_flush(writer); */
	fclose(writer->file);
	free(writer);
}

void delete_wav_writer(struct wav_writer *writer) {
	/* wav_writer_flush(writer); */
	free(writer);
}
