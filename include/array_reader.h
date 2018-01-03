/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    array_reader.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/01 2018 14:38
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/01 2018      create the file
 * 
 *     last modified: 03/01 2018 14:38
 */
#ifndef _BASE_LIB_ARRAY_READER_H_
#define _BASE_LIB_ARRAY_READER_H_

#ifdef __cplusplus
extern "C" {
#endif

struct array_reader;

struct array_reader *create_array_reader(const char *buf, int size);
void delete_array_reader(struct array_reader *reader);
void array_reader_dump_buffer(struct array_reader *reader);

const char *array_reader_get_buffer(struct array_reader *reader);
int array_reader_get_offset(struct array_reader *reader);
int array_reader_skip_offset(struct array_reader *reader, int offset);
int array_reader_get_size(struct array_reader *reader);

int array_reader_peek_type(struct array_reader *reader);
int array_reader_peek_string_size(struct array_reader *reader);
int array_reader_peek_array_size(struct array_reader *reader);

char array_reader_read_byte(struct array_reader *reader);
int array_reader_read_int(struct array_reader *reader);
short array_reader_read_short(struct array_reader *reader);
long long array_reader_read_long(struct array_reader *reader);
float array_reader_read_float(struct array_reader *reader);
double array_reader_read_double(struct array_reader *reader);
void array_reader_read_string(struct array_reader *reader, char *val, int size);

void array_reader_read_byte_array(struct array_reader *reader, char val[], int length);
void array_reader_read_int_array(struct array_reader *reader, int val[], int length);
void array_reader_read_short_array(struct array_reader *reader, short val[], int length);
void array_reader_read_long_array(struct array_reader *reader, long long val[], int length);
void array_reader_read_float_array(struct array_reader *reader, float val[], int length);
void array_reader_read_double_array(struct array_reader *reader, double val[], int length);

#ifdef __cplusplus
}
#endif

#endif
