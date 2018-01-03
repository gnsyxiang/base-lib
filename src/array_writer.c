/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    array_writer.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/01 2018 14:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/01 2018      create the file
 * 
 *     last modified: 03/01 2018 14:43
 */
#include <stdio.h>
#include <string.h>

#include "array_writer.h"
#include "array.h"

struct array_writer *create_array_writer(int capacity) {
    return (struct array_writer *) create_array(capacity);
}

void delete_array_writer(struct array_writer *writer) {
    delete_array((struct array *)writer);
}

void array_writer_dump_buffer(struct array_writer *writer) {
    int i;
    const unsigned char *buf =
            (const unsigned char *)array_writer_get_buffer(writer);
    int size = array_writer_get_size(writer);

    log_e("==============%s==========\n", __func__);

    for (i = 0; i < size; i++) {
        log_e("0x%02x, ", buf[i]);
        if (!(i % 16))
            log_e("0x%02x\n", buf[i]);
    }

    log_e("\n=========================\n");
}

const char *array_writer_get_buffer(struct array_writer *writer) {
    return array_get_buffer((struct array *)writer);
}

int array_writer_get_size(struct array_writer *writer) {
    return array_get_offset((struct array *)writer);
}

static void write_int(struct array_writer *writer, int val) {
    array_write((struct array *)writer, &val, sizeof(int));
}

void array_writer_write_byte(struct array_writer *writer, char val) {
    write_int(writer, TYPE_BYTE);

    array_write((struct array *)writer, &val, sizeof(char));
}

void array_writer_write_int(struct array_writer *writer, int val) {
    write_int(writer, TYPE_INT);

    array_write((struct array *)writer, &val, sizeof(int));
}

void array_writer_write_short(struct array_writer *writer, short val) {
    write_int(writer, TYPE_SHORT);

    array_write((struct array *)writer, &val, sizeof(short));
}

void array_writer_write_long(struct array_writer *writer, long long val) {
    write_int(writer, TYPE_LONG);

    array_write((struct array *)writer, &val, sizeof(long long));
}

void array_writer_write_float(struct array_writer *writer, float val) {
    write_int(writer, TYPE_FLOAT);

    array_write((struct array *)writer, &val, sizeof(float));
}

void array_writer_write_double(struct array_writer *writer, double val) {
    write_int(writer, TYPE_DOUBLE);

    array_write((struct array *)writer, &val, sizeof(double));
}

void array_writer_write_string(struct array_writer *writer, const char *val) {
    write_int(writer, TYPE_STRING);
    write_int(writer, strlen(val) + 1);

    array_write((struct array *)writer, val, strlen(val) + 1);
}


void array_writer_write_byte_array(
        struct array_writer *writer, const char val[], int length) {
    write_int(writer, TYPE_BYTE_ARRAY);
    write_int(writer, length);

    array_write((struct array *)writer, val, sizeof(char) * length);
}

void array_writer_write_int_array(
        struct array_writer *writer, const int val[], int length) {
    write_int(writer, TYPE_INT_ARRAY);
    write_int(writer, length);

    array_write((struct array *)writer, val, sizeof(int) * length);
}

void array_writer_write_short_array(
        struct array_writer *writer, const short val[], int length) {
    write_int(writer, TYPE_SHORT_ARRAY);
    write_int(writer, length);

    array_write((struct array *)writer, val, sizeof(short) * length);
}

void array_writer_write_long_array(
        struct array_writer *writer, const long long val[], int length) {
    write_int(writer, TYPE_LONG_ARRAY);
    write_int(writer, length);

    array_write((struct array *)writer, val, sizeof(long long) * length);
}

void array_writer_write_float_array(
        struct array_writer *writer, const float val[], int length) {
    write_int(writer, TYPE_FLOAT_ARRAY);
    write_int(writer, length);

    array_write((struct array *)writer, val, sizeof(float) * length);
}

void array_writer_write_double_array(
        struct array_writer *writer, const double val[], int length) {
    write_int(writer, TYPE_DOUBLE_ARRAY);
    write_int(writer, length);

    array_write((struct array *)writer, val, sizeof(double) * length);
}
