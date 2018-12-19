/**
 * 
 * Release under GPLv2.
 * 
 * @file    rb-list.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/12 2018 17:33
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/12 2018      create the file
 * 
 *     last modified: 17/12 2018 17:33
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>

#include "list.h"
#include "time_helper.h"
#include "mem_helper.h"
#include "log_helper.h"
#include "rb-list.h"

typedef struct rb_list_node {
    char *buf;
    int len;

    struct list_head list;
}rb_list_node_t;

typedef struct rb_list_head {
    struct list_head list;

    pthread_mutex_t mutex;
    pthread_cond_t in_cond;
    pthread_cond_t out_cond;

    unsigned int in_sleep_cnt;
    unsigned int out_sleep_cnt;

    unsigned int pic_num;
} rb_list_head_t;

#define RB_LIST_NODE_LEN    sizeof(struct rb_list_node)
#define RB_LIST_HEAD_LEN    sizeof(struct rb_list_head)

rb_list_head_t *rb_list_init(void)
{
    rb_list_head_t *rb_list_head = alloc_mem(RB_LIST_HEAD_LEN);
    if (!rb_list_head) {
        log_e("alloc_mem faild");
        return NULL;
    }

    rb_list_head->in_sleep_cnt = 0;
    rb_list_head->out_sleep_cnt = 0;
    rb_list_head->pic_num = 0;

    INIT_LIST_HEAD(&rb_list_head->list);

    pthread_mutex_init(&rb_list_head->mutex, NULL);
    pthread_cond_init(&rb_list_head->in_cond, NULL);
    pthread_cond_init(&rb_list_head->out_cond, NULL);

    return rb_list_head;
}

void rb_list_clean(rb_list_head_t *rb_list_head)
{
    rb_list_node_t *tmp, *tmp2;

    pthread_mutex_lock(&rb_list_head->mutex);

    list_for_each_entry_safe(tmp, tmp2, &rb_list_head->list, list) {
        free(tmp->buf);
        list_del(&tmp->list);
    }

    pthread_mutex_unlock(&rb_list_head->mutex);

    pthread_mutex_destroy(&rb_list_head->mutex);
    pthread_cond_destroy(&rb_list_head->in_cond);
    pthread_cond_destroy(&rb_list_head->out_cond);

    free_mem(rb_list_head);
}

int rb_list_put(rb_list_head_t *rb_list_head, char *buf, int len)
{
    rb_list_node_t *new = alloc_mem(RB_LIST_NODE_LEN);
    if (!new) {
        log_e("alloc_mem faild");
        return -1;
    }

    new->buf = alloc_mem(len);
    if (!new->buf) {
        log_e("alloc_mem faild");
        return -2;
    }

    memcpy(new->buf, buf, len);
    new->len = len;

    pthread_mutex_lock(&rb_list_head->mutex);

    list_add_tail(&(new->list), &(rb_list_head->list));

    if (rb_list_head->pic_num == 0) {
        pthread_cond_broadcast(&rb_list_head->in_cond);
    }

    rb_list_head->pic_num++;

    pthread_mutex_unlock(&rb_list_head->mutex);

    return 0;
}

int rb_list_get(rb_list_head_t *rb_list_head, char *buf, int *len)
{
    rb_list_node_t *tmp, *tmp2;
    int timeout_ms = INT_MAX;

    pthread_mutex_lock(&rb_list_head->mutex);

    if (rb_list_head->pic_num <= 0) {
        struct timespec ts = cur_delay_ms(timeout_ms);
        int ret = pthread_cond_timedwait(&rb_list_head->in_cond,
                &rb_list_head->mutex, &ts);    //receive the signal and return 0

        /* if timed out, discard this data */
        if (ret) {
            pthread_mutex_unlock(&rb_list_head->mutex);
            return -ret;
        }
    }

    list_for_each_entry_safe(tmp, tmp2, &rb_list_head->list, list) {
        memcpy(buf, tmp->buf, tmp->len);
        *len = tmp->len;

        free(tmp->buf);
        list_del(&tmp->list);
        break;
    }

    rb_list_head->pic_num--;

    pthread_mutex_unlock(&rb_list_head->mutex);

    return 0;
}

