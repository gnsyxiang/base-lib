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
#include <pthread.h>

#define RINGBUF_GB
#include "ringbuf.h"
#undef RINGBUF_GB

#include "log_helper.h"
#include "time_helper.h"
#include "typedef_helper.h"
#include "mem_helper.h"

struct ringbuf {
    uint32_t head;
    uint32_t tail;

    uint32_t size;
    uint32_t remain_size;

    pthread_mutex_t mutex;
    pthread_cond_t in_cond;
    pthread_cond_t out_cond;

    uint32_t in_sleep_cnt;
    uint32_t out_sleep_cnt;

    char buf[0];                // 不用指针，如果是协议的话，方便整包发送
};

#define RINGBUF_LEN     (sizeof(struct ringbuf))

prb_t rb_init(uint32_t size)
{
    if (size < 0) {
        log_e("size is less than zero");
        return NULL;
    }

    size = ALIGN4(size);
    prb_t rb = (prb_t)alloc_mem(RINGBUF_LEN + size);
    if (!rb) {
        log_e("malloc faild");
        return NULL;
    }
    
    rb->head = 0;
    rb->tail = 0;

    rb->size = size;
    rb->remain_size = size;

    pthread_mutex_init(&rb->mutex, NULL);
    pthread_cond_init(&rb->in_cond, NULL);
    pthread_cond_init(&rb->out_cond, NULL);

    return rb;
}

void rb_clean(prb_t rb)
{
    if (!rb) {
        log_e("ringbuf is NULL");
        return;
    }

    pthread_mutex_destroy(&rb->mutex);
    pthread_cond_destroy(&rb->in_cond);
    pthread_cond_destroy(&rb->out_cond);

    free(rb);
}

int32_t rb_in_timeout(prb_t rb, void *buf, uint32_t size, uint32_t timeout_ms)
{
    int32_t tail;

    pthread_mutex_lock(&rb->mutex);

    /* the ringbuf can't save the current data */
    while (size > rb->remain_size) {
        struct timespec ts = cur_delay_ms(timeout_ms);
        int32_t ret;

        rb->in_sleep_cnt++;
        /* receive the signal and return 0 */
        ret = pthread_cond_timedwait(&rb->out_cond, &rb->mutex, &ts);
        rb->in_sleep_cnt--;

        /* if timed out, discard this data */
        if (ret) {
            pthread_mutex_unlock(&rb->mutex);
            return -ret;
        }
    }

    tail = rb->tail;

    if (tail + size < rb->size) {
        memcpy(rb->buf + tail, buf, size);
        rb->tail = tail + size;
    } else {
        int32_t sz = rb->size - tail;

        memcpy(rb->buf + tail, buf, sz);
        memcpy(rb->buf, buf + sz, size - sz);

        rb->tail = size - sz;
    }

    if (rb->remain_size > 0) {
        rb->remain_size -= size;
    }

    if (rb->out_sleep_cnt) {
        pthread_cond_broadcast(&rb->in_cond);
    }

    pthread_mutex_unlock(&rb->mutex);

    return size;
}

static int32_t rb_out_timeout_base(
        prb_t rb,
        void *buf,
        uint32_t size,
        uint32_t timeout_ms,
        int32_t del_data_flag)
{
    int head;

    pthread_mutex_lock(&rb->mutex);

    /* printf("size: %d, remain_size: %d \n", rb->size, rb->remain_size); */

    /* there is not enough data in the ringbuf */
    while (rb->size - rb->remain_size < size) {
        struct timespec ts = cur_delay_ms(timeout_ms);
        int ret;

        rb->out_sleep_cnt++;
        /* receive the signal and return 0 */
        ret = pthread_cond_timedwait(&rb->in_cond, &rb->mutex, &ts);    
        rb->out_sleep_cnt--;

        /* if timed out, discard this data */
        if (ret) {
            pthread_mutex_unlock(&rb->mutex);
            return -ret;
        }
    }

    head = rb->head;

    if (head + size < rb->size) {
        memcpy(buf, rb->buf + head, size);

        if (del_data_flag)
            rb->head = head + size;
    } else {
        int sz = rb->size - head;

        memcpy(buf, rb->buf + head, sz);
        memcpy(buf + sz, rb->buf, size - sz);

        if (del_data_flag)
            rb->head = size - sz;
    }

    if (del_data_flag) {
        if (rb->remain_size < rb->size) {
            rb->remain_size += size;
        }

        if (rb->in_sleep_cnt) {
            pthread_cond_broadcast(&rb->out_cond);
        }
    }

    pthread_mutex_unlock(&rb->mutex);

    return size;
}

int32_t rb_out_timeout(prb_t rb, void *buf, uint32_t size, uint32_t timeout_ms)
{
    return rb_out_timeout_base(rb, buf, size, timeout_ms, 1);
}

int32_t rb_out_peek_timeout(prb_t rb, void *buf, uint32_t size, uint32_t timeout_ms)
{
    return rb_out_timeout_base(rb, buf, size, timeout_ms, 0);
}

int32_t rb_remove(prb_t rb, uint32_t size)
{
    pthread_mutex_lock(&rb->mutex);

    if (size < rb->size - rb->remain_size) {
        rb->head = (rb->head + size) % rb->size;
        rb->remain_size += size;
    } else {
        size = rb->size - rb->remain_size;
        rb->head = rb->tail;
        rb->remain_size = rb->size;
    }

    if (rb->in_sleep_cnt) {
        pthread_cond_broadcast(&rb->out_cond);
    }

    pthread_mutex_unlock(&rb->mutex);

    return size;
}

uint32_t rb_is_empty(prb_t rb)
{
    return (rb->remain_size == rb->size)? 1 : 0;
}

uint32_t rb_is_full(prb_t rb)
{
    return (rb->remain_size == 0)? 1 : 0;
}

uint32_t rb_remain_size(prb_t rb)
{
    return rb->remain_size;
}

