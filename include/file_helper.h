/**
 *
 * Release under GPLv2.
 * 
 * @file    file_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 15:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 15:32
 */
#ifndef _FILE_HELPER_H_
#define _FILE_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "log_helper.h"

#ifndef FILE_HELPER_GB
#define FILE_HELPER_EX extern
#else
#define FILE_HELPER_EX
#endif

static inline void _fclose_l(FILE **fp)
{
	if(*fp) {
		fclose(*fp);
		*fp = NULL;
	}
}
#define fclose_l(fp) _fclose_l(&fp)

static inline FILE *fopen_l(const char *path, const char *mode)
{
	FILE *fp = fopen(path, mode);
	if (NULL == fp) {
		log_e("fopen faild");
		return NULL;
	}

	return fp;
}

FILE_HELPER_EX int file_close_on_exec(int fd);
FILE_HELPER_EX int file_set_nonblocking(int fd);
FILE_HELPER_EX ssize_t file_read(int fd, void *buf, size_t cnt);
FILE_HELPER_EX ssize_t file_write(int fd, const void *buf, size_t cnt);


#ifdef __cplusplus
}
#endif

#endif

