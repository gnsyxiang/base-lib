/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    dir_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:29
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:29
 */
#ifndef _DIR_HELPER_H_
#define _DIR_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DIR_HELPER_GB
#define DIR_HELPER_EX extern
#else
#define DIR_HELPER_EX
#endif

typedef void (*handle)(const char* base_path, const char *name);


DIR_HELPER_EX void read_file_list(char *basePath, handle handle_file_dir);


#ifdef __cplusplus
}
#endif

#endif /* end _DIR_HELPER_H_ */

