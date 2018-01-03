/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    array.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/01 2018 14:44
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/01 2018      create the file
 * 
 *     last modified: 03/01 2018 14:44
 */
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "array.h"

#define DEFAULT_SIZE 512

struct array {
    char *buffer;
    int offset;
    int capacity;
};

static void extent_capacity(struct array *array, int size) {
    if (array->offset + size > array->capacity) {
        int new_capacity = (array->offset + size) * 3 / 2;

        array->buffer = realloc(array->buffer, new_capacity);
        assert(array->buffer != NULL);
        array->capacity = new_capacity;
    }
}

int array_write(struct array *array, const void *buf, int size) {
    extent_capacity(array, size);

    memcpy(array->buffer + array->offset, buf, size);
    array->offset += size;

    return size;
}

int array_read(struct array *array, void *buf, int size) {

    if (array->capacity - array->offset < size)
        return -1;

    memcpy(buf, array->buffer + array->offset, size);
    array->offset += size;

    return size;
}

int array_peek(struct array *array, void *buf, int size) {

    if (array->capacity - array->offset < size)
        return -1;

    memcpy(buf, array->buffer + array->offset, size);

    return size;
}

struct array *create_array(int size) {
    struct array *array =
            (struct array *)malloc(sizeof(struct array));
    assert(array != NULL);

    array->offset = 0;
    array->capacity = size;
    array->buffer = NULL;
    if (size) {
        array->buffer = (char *)malloc(size);
        assert(array->buffer != NULL);
    }

    return array;
}

struct array *create_array_by_buffer(const char *buf, int size) {
    struct array *array = create_array(size);

    memcpy(array->buffer, buf, size);
    return array;
}

void delete_array(struct array *array) {
    free(array->buffer);
    free(array);
}

const char *array_get_buffer(struct array *array) {
    return array->buffer;
}

int array_get_offset(struct array *array) {
    return array->offset;
}

int array_skip_offset(struct array *array, int offset) {
    if (array->offset + offset > array->capacity)
        return -1;

    array->offset += offset;
    return array->offset;
}

int array_set_offset(struct array *array, int offset) {
    if (offset < 0 || offset > array->capacity)
        return -1;

    array->offset = offset;

    return array->offset;
}

int array_get_capacity(struct array *array) {
    return array->capacity;
}

int array_is_empty(struct array *array)
{
	return (array->offset == array->capacity) ? 1 : 0;
}


