/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    link_manager.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/12 2017 16:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/12 2017      create the file
 * 
 *     last modified: 29/12 2017 16:48
 */
#ifndef IPC_LINK_MANAGER_H
#define IPC_LINK_MANAGER_H
#include <stdlib.h>
#include "list.h"
#include "link.h"

struct link_manager;

typedef void (*link_manager_connect_callback_t)(
        struct link_manager *lm, struct link *link);

struct link_manager {
    const char *name;
    struct link *link;

    pthread_mutex_t lock;
    struct list_head list;

    int is_running;
    pthread_t tid;
    long private_data;
    link_manager_connect_callback_t func;
};

struct link_manager *create_link_manager(const char *name,
        link_manager_connect_callback_t cb, long private_data);

void disconnect_link_manager(struct link_manager *lm);
void delete_link_manager(struct link_manager *lm);

void link_manager_close_link_no_lock(
        struct link_manager *lm, struct link *link);

long link_manager_get_private_data(struct link_manager *lm);

struct list_head *link_manager_get_links(struct link_manager *lm);
void link_manager_put_links(struct link_manager *lm);

#endif
