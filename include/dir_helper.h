/**
 *
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

#include <dirent.h>

#ifndef DIR_HELPER_GB
#define DIR_HELPER_EX extern
#else
#define DIR_HELPER_EX
#endif

typedef int (*filter_t)(const struct dirent *file);
typedef void (*handle_dir_file_t)(const char *base_path, const char *name, unsigned char d_type, void *args);

/**
 * @brief traversing directory files, recursively calling
 *
 * @param base_path: specified directory
 * @param handle_file: callback func
 * @param handle_dir: callback func
 *
 * @return none
 */
DIR_HELPER_EX void read_file_list(const char *base_path, handle_dir_file_t handle_cb, void *args);

/**
 * @brief scan the directory and process the file according to the filter rules
 *
 * @param dir_name: directory name
 * @param filter: the callback func of the filter rules
 * @param handle_file: callback func
 */
DIR_HELPER_EX void scan_dir_sort_file(char *dir_name, filter_t filter, 
		handle_dir_file_t handle_cb, void *args);

#ifdef __cplusplus
}
#endif

#endif /* end _DIR_HELPER_H_ */

