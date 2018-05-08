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

typedef int (*strcmp_t)(const char*, const char*);

/**
 * @brief first and end exchange string
 *
 * @param str: the switched string
 */


STR_HELPER_EX void str_swap_first_and_end(char *str);

/**
 * @brief get the extension name
 *
 * @param file_name: file path
 * @param ext_name: the extension name of the file 
 */
STR_HELPER_EX void str_get_file_extension_name(const char *, char *);

STR_HELPER_EX void str_get_file_name_no_extension_name(const char *, char *);

/**
 * @brief replace the specified string in a string 
 *
 * @param src: original string
 * @param oldstr: old string
 * @param newstr: new string
 * @param num: replace times
 *
 * @return return the new string after the replacement
 */
const char *str_replace_substr(const char *src, const char *oldstr, const char *newstr, int num);

#ifdef __cplusplus
}
#endif

#endif /* _STR_HELPER_H_ */

