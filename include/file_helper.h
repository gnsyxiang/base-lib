/**
 * Copyright (C) 2017 xxx Co., Ltd.
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

#ifndef FILE_HELPER_GB
#define FILE_HELPER_EX extern
#else
#define FILE_HELPER_EX
#endif

FILE_HELPER_EX FILE *fopen_l(const char *path, const char *mode);

FILE_HELPER_EX int file_close_on_exec(int fd);
FILE_HELPER_EX int file_set_nonblocking(int fd);
FILE_HELPER_EX int file_read(int fd, char *buf, int size);
FILE_HELPER_EX int file_write(int fd, const char *buf, int size);


#ifdef __cplusplus
}
#endif

#endif

