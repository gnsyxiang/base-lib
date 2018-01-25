/**
 *
 * Release under GPLv2.
 * 
 * @file    str_buf_helper.h
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
#ifndef __BASE_LIB_STR_BUF_HELPER_H__
#define __BASE_LIB_STR_BUF_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef STR_BUF_HELPER_GB
#define STR_BUF_HELPER_EX extern
#else
#define STR_BUF_HELPER_EX
#endif

typedef struct str_buf_tag {
	char *buf;
	int len;
	int size;
} str_buf_t;

#define str_buf_t_LEN (sizeof(struct str_buf_tag))

str_buf_t *str_buf_create(void);
str_buf_t *str_buf_create_by_len(int len);
str_buf_t *str_buf_create_by_buf(const char *buf);
void str_buf_free_buf(str_buf_t *str);
void str_buf_free(str_buf_t *str);
void str_buf_dump(str_buf_t *str);
void str_buf_dump_buf(str_buf_t *str);

int str_buf_insert_char(str_buf_t *str, char c);
int str_buf_insert_buf(str_buf_t *str, char *buf);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_STR_BUF_HELPER_H__ */

