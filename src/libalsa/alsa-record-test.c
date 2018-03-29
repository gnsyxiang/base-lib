/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa-record-test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/03 2018 16:35
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/03 2018      create the file
 * 
 *     last modified: 26/03 2018 16:35
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#include "alsa-record.h"

void alsa_put_record_result(record_result_t record_result)
{
	if(record_result.data_buf) {
		free(record_result.data_buf);
		record_result.data_buf = NULL;
	}
	record_result.data_buf_size = 0;
}

record_result_t alsa_get_record_result(record_handle_t *record_handle)
{
	record_result_t record_result;
	//计算录frame_num个frame需要的byte数
	record_result.data_buf_size = record_handle->frame_num * record_handle->bits_per_frame / 8;

	//开辟录音buffer
	record_result.data_buf = (uint8_t *)malloc(record_result.data_buf_size);
	if (!record_result.data_buf) {
		printf("Error malloc: [data_buf]");
		record_result.data_buf_size = 0;
	}

	return record_result;
}

void do_get_data(record_handle_t *record_handle, int fd, int n)
{
	record_result_t record_result = alsa_get_record_result(record_handle);

	while (n--) {
		if (read_pcm(record_handle, record_result) != record_handle->frame_num)
			break;
		write(fd, record_result.data_buf, record_result.data_buf_size);
	}

	alsa_put_record_result(record_result);
}

int main(int argc, char *argv[])
{
	int fd = -1;
	record_params_t record_params;

	record_params.snd_dev_name	= "default";
	record_params.format		= SND_PCM_FORMAT_S16_LE;
	record_params.channels		= 2;
	record_params.sample_rate	= 16000;

	record_handle_t *record_handle = alsa_get_record_handle(record_params);
	if(!record_handle){
		fprintf(stderr, "record_handle is NULL.\n");
		return -1;
	}

	alsa_set_read_frame(record_handle, 1024);

#define FILE_PATH	"./haha.pcm"
	remove(FILE_PATH);
	fd = open(FILE_PATH, O_CREAT | O_WRONLY, 777);
	if(fd < 0){
		perror("open");
	}

	do_get_data(record_handle, fd, 50);

	close(fd);
	alsa_put_record_handle(record_handle);
	return 0;
}
