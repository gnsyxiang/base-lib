/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    array_reader.c
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
#include <assert.h>

#include "array_reader.h"
#include "array.h"
#include "hex_helper.h"

struct array_reader *create_array_reader(const char *buf, int size) {
    return (struct array_reader *) create_array_by_buffer(buf, size);
}

void delete_array_reader(struct array_reader *reader) {
    delete_array((struct array *)reader);
}

void array_reader_dump_buffer(struct array_reader *reader) {
    const unsigned char *buf = (const unsigned char *)array_reader_get_buffer(reader);
    int size = array_reader_get_size(reader);

    printf("==============%s==========\n", __func__);
	print_hex_16cnt_of_line(buf, size);
    printf("==========================\n");
}

int array_reader_is_empty(struct array_reader *reader)
{
	return array_is_empty((struct array *)reader);
}

const char *array_reader_get_buffer(struct array_reader *reader) {
    return array_get_buffer((struct array *)reader);
}

int array_reader_get_offset(struct array_reader *reader) {
    return array_get_offset((struct array *)reader);
}

int array_reader_get_size(struct array_reader *reader) {
    return array_get_capacity((struct array *)reader);
}

int array_reader_skip_offset(struct array_reader *reader, int offset) {
    return array_skip_offset((struct array *)reader, offset);
}

static int read_int(struct array_reader *reader) {
    int type, status;

    status = array_read((struct array *)reader, &type, sizeof(int));
    if (status < 0)
        return status;

    return type;
}

int array_reader_peek_type(struct array_reader *reader) {
    int type, status;

    status = array_peek((struct array *)reader, &type, sizeof(int));
    if (status < 0)
        return status;

    return type;
}

int array_reader_peek_string_size(struct array_reader *reader) {
    int msg[2];

    int status = array_peek((struct array *)reader, &msg, sizeof(int) * 2);
    if (status < 0 || msg[0] != TYPE_STRING) {
        log_e("%s: status: %d, type: %d, size: %d\n",
                __func__, status, msg[0], msg[1]);
        assert(0);
    }

    return msg[1];
}

int array_reader_peek_array_size(struct array_reader *reader) {
    int msg[2];

    int status = array_peek((struct array *)reader, &msg, sizeof(int) * 2);
    if (status < 0 || msg[0] < TYPE_BYTE_ARRAY || msg[0] > TYPE_DOUBLE_ARRAY) {
        log_e("%s: status: %d, type: %d, size: %d\n",
                __func__, status, msg[0], msg[1]);
        assert(0);
    }

    return msg[1];
}

char array_reader_read_byte(struct array_reader *reader) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_BYTE) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    char val;
    array_read((struct array *)reader, &val, sizeof(char));

    return val;
}

int array_reader_read_int(struct array_reader *reader) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_INT) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    int val;
    array_read((struct array *)reader, &val, sizeof(int));

    return val;
}

short array_reader_read_short(struct array_reader *reader) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_SHORT) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    short val;
    array_read((struct array *)reader, &val, sizeof(short));

    return val;
}

long long array_reader_read_long(struct array_reader *reader) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_LONG) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    long long val;
    array_read((struct array *)reader, &val, sizeof(long long));

    return val;
}

float array_reader_read_float(struct array_reader *reader) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_FLOAT) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    float val;
    array_read((struct array *)reader, &val, sizeof(float));

    return val;
}

double array_reader_read_double(struct array_reader *reader) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_DOUBLE) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    double val;
    array_read((struct array *)reader, &val, sizeof(double));

    return val;
}

void array_reader_read_string(struct array_reader *reader, char *val, int size) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_STRING) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    int sz = read_int(reader);
    if (sz < 0) {
        log_e("%s: read string size error\n", __func__);
        assert(0);
    }

    if (size != sz) {
        log_e("%s: argument size error, size: %d, string size: %d\n",
                __func__, size, sz);
        assert(0);
    }

    int status = array_read((struct array *)reader, val, size);
    if (status < 0) {
        log_e("%s: read string error\n", __func__);
        assert(0);
    }
}

void array_reader_read_byte_array(
        struct array_reader *reader, char val[], int length) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_BYTE_ARRAY) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    int len = read_int(reader);
    if (len < 0) {
        log_e("%s: read string size error\n", __func__);
        assert(0);
    }

    if (length != len) {
        log_e("%s: argument length error, length: %d, array len: %d\n",
                __func__, length, len);
        assert(0);
    }

    array_read((struct array *)reader, val, sizeof(char) * length);
}

void array_reader_read_int_array(
        struct array_reader *reader, int val[], int length) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_INT_ARRAY) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    int len = read_int(reader);
    if (len < 0) {
        log_e("%s: read string size error\n", __func__);
        assert(0);
    }

    if (length != len) {
        log_e("%s: argument length error, length: %d, array len: %d\n",
                __func__, length, len);
        assert(0);
    }

    array_read((struct array *)reader, val, sizeof(int) * length);
}

void array_reader_read_short_array(
        struct array_reader *reader, short val[], int length) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_SHORT_ARRAY) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    int len = read_int(reader);
    if (len < 0) {
        log_e("%s: read string size error\n", __func__);
        assert(0);
    }

    if (length != len) {
        log_e("%s: argument length error, length: %d, array len: %d\n",
                __func__, length, len);
        assert(0);
    }

    array_read((struct array *)reader, val, sizeof(short) * length);
}

void array_reader_read_long_array(
        struct array_reader *reader, long long val[], int length) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_LONG_ARRAY) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    int len = read_int(reader);
    if (len < 0) {
        log_e("%s: read string size error\n", __func__);
        assert(0);
    }

    if (length != len) {
        log_e("%s: argument length error, length: %d, array len: %d\n",
                __func__, length, len);
        assert(0);
    }

    array_read((struct array *)reader, val, sizeof(long long) * length);
}

void array_reader_read_float_array(
        struct array_reader *reader, float val[], int length) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_FLOAT_ARRAY) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    int len = read_int(reader);
    if (len < 0) {
        log_e("%s: read string size error\n", __func__);
        assert(0);
    }

    if (length != len) {
        log_e("%s: argument length error, length: %d, array len: %d\n",
                __func__, length, len);
        assert(0);
    }

    array_read((struct array *)reader, val, sizeof(float) * length);
}

void array_reader_read_double_array(
        struct array_reader *reader, double val[], int length) {
    int type = read_int(reader);
    if (type < 0 || type != TYPE_DOUBLE_ARRAY) {
        log_e("%s: read type error, type: %d\n", __func__, type);
        assert(0);
    }

    int len = read_int(reader);
    if (len < 0) {
        log_e("%s: read string size error\n", __func__);
        assert(0);
    }

    if (length != len) {
        log_e("%s: argument length error, length: %d, array len: %d\n",
                __func__, length, len);
        assert(0);
    }

    array_read((struct array *)reader, val, sizeof(double) * length);
}
