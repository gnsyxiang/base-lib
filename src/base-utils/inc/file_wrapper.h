/**
 * 
 * Release under GPLv2.
 * 
 * @file    file_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/08 2019 15:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/08 2019      create the file
 * 
 *     last modified: 12/08 2019 15:13
 */
#ifndef __BASE_LIB_UTILS_INC_FILE_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_FILE_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_UTILS_SRC_FILE_WRAPPER_GB
#define BASE_LIB_UTILS_SRC_FILE_WRAPPER_EX extern
#else
#define BASE_LIB_UTILS_SRC_FILE_WRAPPER_EX
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
		printf("fopen %s faild \n", path);
		return NULL;
	}

	return fp;
}

int file_close_on_exec(int fd);
int file_set_nonblocking(int fd);
ssize_t file_read(int fd, void *buf, size_t cnt);
ssize_t file_write(int fd, const void *buf, size_t cnt);
ssize_t file_read_timeout(int fd, void *buf, size_t cnt, size_t timeout_ms);

/**
 * @brief get the length of the file
 *
 * @param path: file name
 *
 * @return: successful, return total length of a file; faild, return -1 
 */
long open_file_get_len(const char *path);


#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_FILE_WRAPPER_H_ */

