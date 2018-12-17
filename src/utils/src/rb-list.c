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

#include "list.h"
#include "mem_helper.h"
#include "log_helper.h"
#include "rb-list.h"

typedef struct rb_list_node {
    char *buf;
    int len;

    pthread_mutex_t mutex;
    struct list_head list;
}rb_list_node_t;

#define RB_LIST_NODE_LEN    sizeof(struct rb_list_node)

static uint32_t pic_num;
static rb_list_node_t rb_list_head;

void rb_list_init(void)
{
    pthread_mutex_init(&rb_list_head.mutex, NULL);
    INIT_LIST_HEAD(&rb_list_head.list);
}

int rb_list_clean(void)
{
    return 0;
}

int rb_list_put(char *buf, int len)
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

    list_add_tail(&(new->list), &(rb_list_head.list));

    return 0;
}

int rb_list_get(char *buf, int *len)
{
    rb_list_node_t *tmp = NULL;

    list_for_each_entry(tmp, &(rb_list_head.list), list) {
        memcpy(buf, tmp->buf, tmp->len);
        *len = tmp->len;
        break;
    }

    return 0;
}

