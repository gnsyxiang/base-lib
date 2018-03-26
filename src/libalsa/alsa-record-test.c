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

extern char *optarg;

void display_usage(char *bin)
{
	printf("Record data\n");
	printf("\t-f: Specified function [a, b, c]\n");
	printf("\ta: use alsa_record_get_data, b: alsa_record_by_time, c: alsa_record_by_size\n");
	printf("\t-t: Specified time\n");
	printf("\t-s: Specified size\n");
	printf("\teg a: %s -f a\n", bin);
	printf("\teg b: %s -f b -t 10\n", bin);
	printf("\teg c: %s -f c -s 461234\n", bin);
}

void do_get_data(record_handle_t *record_handle, int fd, int n)
{
	record_result_t record_result;
	while(n--){
		record_result = alsa_record_get_data(record_handle);
		if(record_result.data_buf == NULL)
			break;
		write(fd, record_result.data_buf, record_result.data_buf_size);
		alsa_free_record_result(record_result);
	}
}

void do_by_time(record_handle_t *record_handle, int fd, int seconds)
{
	record_result_t record_result;
	record_result = alsa_record_by_time(record_handle, seconds);
	write(fd, record_result.data_buf, record_result.data_buf_size);
	alsa_free_record_result(record_result);
}

void do_by_size(record_handle_t *record_handle, int fd, int size)
{
	record_result_t record_result;
	record_result = alsa_record_by_size(record_handle, size);
	write(fd, record_result.data_buf, record_result.data_buf_size);
	alsa_free_record_result(record_result);
}

int main(int argc, char *argv[])
{
	int fd = -1;
	char * type = NULL;
	int seconds = 0;
	int size = 0;

	record_params_t record_params;

	record_params.snd_dev_name	= "default";
	record_params.format		= SND_PCM_FORMAT_S16_LE;
	record_params.channels		= 2;
	record_params.sample_rate	= 16000;

	int opt;
	while( -1 != (opt = getopt( argc, argv, "f:s:t:"))) {
		switch( opt ) {
		case 'f':
			type = optarg;
			break;
		case 's':
			size = atoi(optarg);
			break;
		case 't':
			seconds = atoi(optarg);
			break;
		case 'h':
		default:
			display_usage(argv[0]);
			return -1;
			break;
		}
	}
	if(0 == type) {
		display_usage(argv[0]);
		return -1;
	}

	record_handle_t *record_handle = alsa_get_record_handle(record_params);
	if(!record_handle){
		fprintf(stderr, "record_handle is NULL.\n");
		return -1;
	}

#define FILE_PATH	"./haha.pcm"
	remove(FILE_PATH);
	fd = open(FILE_PATH, O_CREAT | O_WRONLY, 777);
	if(fd < 0){
		perror("open");
	}
	
	if(0 == strcmp("a", type)) {
		printf("Recording by alsa_record_get_data\n");
		do_get_data(record_handle, fd, 50);
	} else if (0 == strcmp("b", type)) {
		printf("Recording by alsa_record_by_time, seconds is %d\n", seconds);
		do_by_time(record_handle, fd, seconds);
	} else if (0 == strcmp("c", type)) {
		printf("Recording by alsa_record_by_size, size is %d\n", size);
		do_by_size(record_handle, fd, size);
	}
	close(fd);
	alsa_put_record_handle(record_handle);
	return 0;
}
