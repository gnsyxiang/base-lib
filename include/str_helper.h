/**
 *
 * Release under GPLv2.
 * 
 * @file    str_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/01 2018 11:15
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/01 2018      create the file
 * 
 *     last modified: 09/01 2018 11:15
 */
#ifndef __BASE_LIB_STR_HELPER_H__
#define __BASE_LIB_STR_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef STR_HELPER_GB
#define STR_HELPER_EX extern
#else
#define STR_HELPER_EX
#endif

typedef struct str_tag {
	char *buf;
	int len;
	int size;
} str_t;

#define STR_T_LEN (sizeof(struct str_tag))

str_t *str_create(void);
str_t *str_create_by_len(int len);
str_t *str_create_by_buf(const char *buf);
void str_free_buf(str_t *str);
void str_free(str_t *str);
void str_dump(str_t *str);

int str_insert_char(str_t *str, char c);
int str_insert_buf(str_t *str, char *buf);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_STR_HELPER_H__ */

