/**
 * 
 * Release under GPLv2.
 * 
 * @file    ringbuf.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/05 2018 14:15
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/05 2018      create the file
 * 
 *     last modified: 09/05 2018 14:15
 */
#ifndef __BASE_LIB_RINGBUF_H_
#define __BASE_LIB_RINGBUF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <limits.h>

#ifndef RINGBUF_GB
#define RINGBUF_EX extern
#else
#define RINGBUF_EX
#endif

typedef struct ringbuf {
    unsigned int head;
    unsigned int tail;

    unsigned int size;
    unsigned int remain_size;

    pthread_mutex_t mutex;
    pthread_cond_t in_cond;
    pthread_cond_t out_cond;

    unsigned int in_sleep_cnt;
    unsigned int out_sleep_cnt;

    char buf[0];
}ringbuf_t;
typedef struct ringbuf* pringbuf_t;

RINGBUF_EX pringbuf_t ringbuf_init(unsigned int size);
RINGBUF_EX void ringbuf_destroy(pringbuf_t pringbuf);

RINGBUF_EX int ringbuf_is_full(pringbuf_t pringbuf);
RINGBUF_EX int ringbuf_is_empty(pringbuf_t pringbuf);

RINGBUF_EX int ringbuf_in_timeout(pringbuf_t pringbuf, void *buf, unsigned int size, unsigned int timeout_ms);
static inline
int ringbuf_in(pringbuf_t pringbuf, void *buf, unsigned int size)
{
    return ringbuf_in_timeout(pringbuf, buf, size, INT_MAX);
}

RINGBUF_EX int ringbuf_out_timeout(pringbuf_t pringbuf, void *buf, unsigned int size, unsigned int timeout_ms);
static inline
int ringbuf_out(pringbuf_t pringbuf, void *buf, unsigned int size)
{
    return ringbuf_out_timeout(pringbuf, buf, size, INT_MAX);
}

RINGBUF_EX int ringbuf_out_peek_timeout(pringbuf_t pringbuf, void *buf, unsigned int size, unsigned int timeout_ms);
static inline
int ringbuf_out_peek(pringbuf_t pringbuf, void *buf, unsigned int size)
{
    return ringbuf_out_peek_timeout(pringbuf, buf, size, INT_MAX);
}

RINGBUF_EX int ringbuf_remove(pringbuf_t pringbuf, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif /* end _RINGBUF_H_ */

