/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    array_writer.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/01 2018 14:40
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/01 2018      create the file
 * 
 *     last modified: 03/01 2018 14:40
 */
#ifndef _BASE_LIB_ARRAY_WRITER_H_
#define _BASE_LIB_ARRAY_WRITER_H_

#ifdef __cplusplus
extern "C" {
#endif

struct array_writer;

struct array_writer *create_array_writer(int capacity);
void delete_array_writer(struct array_writer *writer);
void array_writer_dump_buffer(struct array_writer *writer);

const char *array_writer_get_buffer(struct array_writer *writer);
int array_writer_get_size(struct array_writer *writer);

void array_writer_write_byte(struct array_writer *writer, char val);
void array_writer_write_int(struct array_writer *writer, int val);
void array_writer_write_short(struct array_writer *writer, short val);
void array_writer_write_long(struct array_writer *writer, long long val);
void array_writer_write_float(struct array_writer *writer, float val);
void array_writer_write_double(struct array_writer *writer, double val);
void array_writer_write_string(struct array_writer *writer, const char *val);

void array_writer_write_byte_array(struct array_writer *writer, const char val[], int length);
void array_writer_write_int_array(struct array_writer *writer, const int val[], int length);
void array_writer_write_short_array(struct array_writer *writer, const short val[], int length);
void array_writer_write_long_array(struct array_writer *writer, const long long val[], int length);
void array_writer_write_float_array(struct array_writer *writer, const float val[], int length);
void array_writer_write_double_array(struct array_writer *writer, const double val[], int length);

#ifdef __cplusplus
}
#endif

#endif
