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
#ifndef __UTILS_DIR_HELPER_H_
#define __UTILS_DIR_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <dirent.h>

#ifndef DIR_HELPER_GB
#define DIR_HELPER_EX extern
#else
#define DIR_HELPER_EX
#endif

/**
 * @brief 过滤接口
 *
 * @param 
 *
 * @return 
 */
typedef int (*filter_t)(const struct dirent *file);

/**
 * @brief 回调接口
 *
 * @param name: 文件名
 * @param d_type: 文件类型
 * @param args: 传递的参数
 *
 * @return 无
 */
typedef void (*dir_file_cb_t)(const char * const name,
        unsigned char d_type, void *args);

/**
 * @brief 读取指定路径下的文件
 *
 * @param path: 路径
 * @param dir_file_cb: 回调函数
 * @param args: 传递的参数
 *
 * @return 成功返回0，失败返回负值
 */
int read_dir_file(const char * const path,
        dir_file_cb_t dir_file_cb, void *args);

/**
 * @brief 读取指定路径下的文件，过滤器版本
 *
 * @param dir: 指定目录
 * @param filter: 过滤器
 * @param dir_file_cb: 回调函数
 * @param args: 传递的参数
 *
 * @return 成功返回0，失败返回负值
 */
int scan_dir_sort_file(const char * const dir, filter_t filter, 
		dir_file_cb_t dir_file_cb, void *args);

#ifdef __cplusplus
}
#endif

#endif /* end _DIR_HELPER_H_ */

