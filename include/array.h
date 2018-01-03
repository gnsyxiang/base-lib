/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    array.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/01 2018 14:37
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/01 2018      create the file
 * 
 *     last modified: 03/01 2018 14:37
 */
#ifndef _BASE_LIB_ARRAY_H_ 
#define _BASE_LIB_ARRAY_H_

#include "log_helper.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    TYPE_BYTE,
    TYPE_INT,
    TYPE_SHORT,
    TYPE_LONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BYTE_ARRAY,
    TYPE_INT_ARRAY,
    TYPE_SHORT_ARRAY,
    TYPE_LONG_ARRAY,
    TYPE_FLOAT_ARRAY,
    TYPE_DOUBLE_ARRAY,
};

struct array;

struct array *create_array(int size);
struct array *create_array_by_buffer(const char *buf, int size);
void delete_array(struct array *array);

int array_write(struct array *writer, const void *buf, int size);
int array_read(struct array *reader, void *buf, int size);
int array_peek(struct array *array, void *buf, int size);

const char *array_get_buffer(struct array *array);
int array_get_offset(struct array *array);
int array_skip_offset(struct array *array, int offset);
int array_set_offset(struct array *array, int offset);
int array_get_capacity(struct array *array);

#ifdef __cplusplus
}
#endif

#endif

