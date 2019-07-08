/**
 *
 * Release under GPLv2.
 * 
 * @file    dir_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/07 2019 17:55
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/07 2019      create the file
 * 
 *     last modified: 02/07 2019 17:55
 */
#ifndef __BASE_LIB_UTILS_INC_DIR_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_DIR_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_UTILS_SRC_DIR_WRAPPER_GB
#define BASE_LIB_UTILS_SRC_DIR_WRAPPER_EX extern
#else
#define BASE_LIB_UTILS_SRC_DIR_WRAPPER_EX
#endif

#include <dirent.h>

#define DOT   "."
#define DOT2  ".."

typedef void (*dir_file_cb_t)(char *name, unsigned char d_type, void *args);
typedef int (*file_filter_t)(const struct dirent *file);

typedef struct handle_dir {
    char * dir_path;
    void * args;
    dir_file_cb_t dir_file_cb;
    file_filter_t file_filter;
} handle_dir_t;

int read_dir(handle_dir_t *handle_dir);
int read_dir_filter_file(handle_dir_t *handle_dir);
int is_dir(const char * const filename);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_DIR_WRAPPER_H_ */

