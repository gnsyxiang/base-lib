/**
 *
 * Release under GPLv2.
 * 
 * @file    str_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/01 2018 10:03
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/01 2018      create the file
 * 
 *     last modified: 26/01 2018 10:03
 */
#ifndef _STR_HELPER_H_
#define _STR_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STR_HELPER_GB
#define STR_HELPER_EX extern
#else
#define STR_HELPER_EX
#endif

STR_HELPER_EX void str_get_file_extension_name(const char *file_name, char *ext_name);

#ifdef __cplusplus
}
#endif

#endif /* _STR_HELPER_H_ */

