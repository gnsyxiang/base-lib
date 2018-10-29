/**
 * 
 * Release under GPLv2.
 * 
 * @file    ringbuf.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/05 2018 14:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/05 2018      create the file
 * 
 *     last modified: 09/05 2018 14:13
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RINGBUF_GB
#include "ringbuf.h"
#undef RINGBUF_GB

#include "time_helper.h"
#include "typedef_helper.h"

static int ringbuf_out_timeout_base(pringbuf_t pringbuf, void *buf,
        unsigned int size, unsigned int timeout_ms, int delete_data_flag);

pringbuf_t ringbuf_init(unsigned int size)
{
    pringbuf_t pringbuf;

    if (size < 0) {
        printf("size is less than zero");
        return NULL;
    }

    size = ALIGN4(size);

    pringbuf = (pringbuf_t)malloc(sizeof(*pringbuf) + size);
    if (!pringbuf) {
        printf("malloc faild \n");
        return NULL;
    }

    pringbuf->head = 0;
    pringbuf->tail = 0;

    pringbuf->size = size;
    pringbuf->remain_size = size;

    pthread_mutex_init(&pringbuf->mutex, NULL);
    pthread_cond_init(&pringbuf->in_cond, NULL);
    pthread_cond_init(&pringbuf->out_cond, NULL);

    return pringbuf;
}

void ringbuf_destroy(pringbuf_t pringbuf)
{
    if (!pringbuf) {
        printf("ringbuf is NULL \n");
        return;
    }

    pthread_mutex_destroy(&pringbuf->mutex);
    pthread_cond_destroy(&pringbuf->in_cond);
    pthread_cond_destroy(&pringbuf->out_cond);

    free(pringbuf);
}

int ringbuf_in_timeout(
        pringbuf_t pringbuf,
        void *buf,
        unsigned int size,
        unsigned int timeout_ms)
{
    int tail;

    pthread_mutex_lock(&pringbuf->mutex);

    /* the ringbuf can't save the current data */
    while (size > pringbuf->remain_size) {
        struct timespec ts = cur_delay_ms(timeout_ms);
        int ret;

        pringbuf->in_sleep_cnt++;
        ret = pthread_cond_timedwait(&pringbuf->out_cond, &pringbuf->mutex, &ts);   //receive the signal and return 0
        pringbuf->in_sleep_cnt--;

        /* if timed out, discard this data */
        if (ret) {
            pthread_mutex_unlock(&pringbuf->mutex);
            return -ret;
        }
    }

    tail = pringbuf->tail;

    if (tail + size < pringbuf->size) {
        memcpy(pringbuf->buf + tail, buf, size);
        pringbuf->tail = tail + size;
    } else {
        int sz = pringbuf->size - tail;

        memcpy(pringbuf->buf + tail, buf, sz);
        memcpy(pringbuf->buf, buf + sz, size - sz);

        pringbuf->tail = size - sz;
    }

    if (pringbuf->remain_size > 0) {
        pringbuf->remain_size -= size;
    }

    if (pringbuf->out_sleep_cnt) {
        pthread_cond_broadcast(&pringbuf->in_cond);
    }

    pthread_mutex_unlock(&pringbuf->mutex);

    return size;
}

int ringbuf_out_timeout(
        pringbuf_t pringbuf,
        void *buf,
        unsigned int size,
        unsigned int timeout_ms)
{
    return ringbuf_out_timeout_base(pringbuf, buf, size, timeout_ms, 1);
}

int ringbuf_out_peek_timeout(
        pringbuf_t pringbuf,
        void *buf,
        unsigned int size,
        unsigned int timeout_ms)
{
    return ringbuf_out_timeout_base(pringbuf, buf, size, timeout_ms, 0);
}

static int ringbuf_out_timeout_base(
        pringbuf_t pringbuf,
        void *buf,
        unsigned int size,
        unsigned int timeout_ms,
        int delete_data_flag)
{
    int head;

    pthread_mutex_lock(&pringbuf->mutex);

    /* printf("size: %d, remain_size: %d \n", pringbuf->size, pringbuf->remain_size); */

    /* there is not enough data in the ringbuf */
    while (pringbuf->size - pringbuf->remain_size < size) {
        struct timespec ts = cur_delay_ms(timeout_ms);
        int ret;

        pringbuf->out_sleep_cnt++;
        ret = pthread_cond_timedwait(&pringbuf->in_cond, &pringbuf->mutex, &ts);    //receive the signal and return 0
        pringbuf->out_sleep_cnt--;

        /* if timed out, discard this data */
        if (ret) {
            pthread_mutex_unlock(&pringbuf->mutex);
            return -ret;
        }
    }

    head = pringbuf->head;

    if (head + size < pringbuf->size) {
        memcpy(buf, pringbuf->buf + head, size);

        if (delete_data_flag)
            pringbuf->head = head + size;
    } else {
        int sz = pringbuf->size - head;

        memcpy(buf, pringbuf->buf + head, sz);
        memcpy(buf + sz, pringbuf->buf, size - sz);

        if (delete_data_flag)
            pringbuf->head = size - sz;
    }

    if (delete_data_flag) {
        if (pringbuf->remain_size < pringbuf->size) {
            pringbuf->remain_size += size;
        }

        if (pringbuf->in_sleep_cnt) {
            pthread_cond_broadcast(&pringbuf->out_cond);
        }
    }

    pthread_mutex_unlock(&pringbuf->mutex);

    return size;
}

int ringbuf_remove(pringbuf_t pringbuf, unsigned int size)
{
    pthread_mutex_lock(&pringbuf->mutex);

    if (size < pringbuf->size - pringbuf->remain_size) {
        pringbuf->head = (pringbuf->head + size) % pringbuf->size;
        pringbuf->remain_size += size;
    } else {
        size = pringbuf->size - pringbuf->remain_size;
        pringbuf->head = pringbuf->tail;
        pringbuf->remain_size = pringbuf->size;
    }

    if (pringbuf->in_sleep_cnt) {
        pthread_cond_broadcast(&pringbuf->out_cond);
    }

    pthread_mutex_unlock(&pringbuf->mutex);

    return size;
}

int ringbuf_is_empty(pringbuf_t pringbuf)
{
    if (pringbuf->remain_size == pringbuf->size) {
        return 1;
    } else {
        return 0;
    }
}

int ringbuf_is_full(pringbuf_t pringbuf)
{
    if (pringbuf->remain_size == 0) {
        return 1;
    } else {
        return 0;
    }
}


