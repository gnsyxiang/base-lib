/**
 *
 * Release under GPLv2.
 * 
 * @file    str_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 10:36
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 10:36
 */

/*
 * #include <string.h>
 * char *strdup(const char *s);
 * 
 * 作用: 将串拷贝到新建的位置处，返回新分配空间的首地址，失败返回NULL
 * 注意: strdup在内部调用malloc()为变量分配内存，不需要使用返回的字符串时，
 *       需要用free()释放相应的内存空间，否则会造成内存泄露
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "log_helper.h"

#define STR_HELPER_GB
#include "str_helper.h"
#undef STR_HELPER_GB

static void *alloc_mem(int len)
{
	void *buf = calloc(1, len);
	if (NULL == buf) {
		log_e("calloc faild");
		return NULL;
	}

	return buf;
}

#define REALLOC_SIZE (32)

static int realloc_mem(void *buf, int len, int size)
{
	for (size = size < REALLOC_SIZE ? REALLOC_SIZE : size; size < len; size <<= 1);	

	char *new_buf = realloc(buf, size);
	if (NULL == new_buf) {
		log_e("realloc faild");
		return -1;
	}

	return size;
}

static void free_mem(void *buf)
{
	if (buf) {
		free(buf);
		buf = NULL;
	}
}

str_t *str_create(void)
{
	return (str_t *)alloc_mem(STR_T_LEN);
}

str_t *str_create_by_len(int len)
{
	str_t *str = str_create();
	str->buf = alloc_mem(len);

	return str;
}

str_t *str_create_by_buf(const char *buf)
{
	str_t *str = str_create();

	int len = strlen(buf);
	if (len) {
		str->buf = alloc_mem(len + 1);

		strcpy(str->buf, buf);
		str->len = len;
	}

	return str;
}

void str_free_buf(str_t *str)
{
	free_mem(str->buf);
}

void str_free(str_t *str)
{
	str_free_buf(str);
	free_mem(str);
}

void str_dump(str_t *str)
{
	if (!str || !str->buf)
		return;

	for (int i = 0; i < str->len; i++) {
		printf("%02x ", str->buf[i]);
	}

	printf("\n");
}

int str_insert_char(str_t *str, char c)
{
	if (!str) {
		log_e("the str is NULL");
		return -1;
	}

	/*str->len指向实际的长度，这里要考虑增加字符和字符结束符(\0)*/
	int len = str->len + 2;

	if ((len > str->size) && (-1 == realloc_mem(str->buf, len, str->size)))
		return -1;

	str->len = --len;
	str->buf[len - 1] = c;
	str->buf[len] = '\0';

	return 0;
}

int str_insert_buf(str_t *str, char *buf)
{
	if (!str || !buf) {
		log_e("the str or buf is NULL");
		return -1;
	}

	int len = str->len + strlen(buf) + 1;
	if ((len > str->size) && (-1 == realloc_mem(str->buf, len, str->size)))
		return -1;

	strcpy(str->buf + str->len, buf);
	str->buf[len] = '\0';
	str->len = len - 1;
	
	return 0;
}









