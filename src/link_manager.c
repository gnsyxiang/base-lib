/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    link_manager.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/12 2017 16:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/12 2017      create the file
 * 
 *     last modified: 29/12 2017 16:20
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

#include "list.h"
#include "log_helper.h"
#include "link_manager.h"
#include "pthread_helper.h"

static void *link_manager_thread_loop(void *user);

struct link_manager *create_link_manager(
		const char *name,
		link_manager_connect_callback_t cb, 
		long private_data)
{
    struct link_manager *lm = (struct link_manager *)malloc(sizeof(struct link_manager));

    lm->link = create_link(name, CONNECTION_SERVER);
    if (lm->link == NULL) {
		log_e("%s: failed to create connection, name: %s\n", __func__, name);
        free(lm);
        return NULL;
    }

    lm->name = name;
    lm->tid = pthread_self();
    lm->func = cb;
    lm->private_data = private_data;
    INIT_LIST_HEAD(&lm->list);
    pthread_mutex_init(&lm->lock, NULL);

    create_a_attached_thread(&lm->tid, link_manager_thread_loop, lm);
    if (lm->tid < 0) {
		log_e("%s: failed to create thread, name: %s\n", __func__, lm->name);
		delete_link_manager(lm);
        return NULL;
    }

    return lm;
}

void disconnect_link_manager(struct link_manager *lm)
{
    struct link *link, *tmp;

    lm->is_running = 0;

    pthread_mutex_lock(&lm->lock);

    if (lm->link)
        link_disconnect(lm->link);

    list_for_each_entry_safe(link, tmp, &lm->list, entry) {
        list_del(&link->entry);

        link_disconnect(link);
        put_link(link);
    }

    pthread_mutex_unlock(&lm->lock);
}

void delete_link_manager(struct link_manager *lm)
{
    disconnect_link_manager(lm);

    int cur_tid = pthread_self();

    if (lm->tid != cur_tid) {
        delete_link(lm->link);
        pthread_join(lm->tid, NULL);
        pthread_mutex_destroy(&lm->lock);

        free(lm);
    }
}

void link_manager_close_link_no_lock(struct link_manager *lm, struct link *link)
{
    list_del(&link->entry);
    link_disconnect(link);

    put_link(link);
}

long link_manager_get_private_data(struct link_manager *lm)
{
    return lm->private_data;
}

struct list_head *link_manager_get_links(struct link_manager *lm)
{
    pthread_mutex_lock(&lm->lock);

    return &lm->list;
}

void link_manager_put_links(struct link_manager *lm)
{
    pthread_mutex_unlock(&lm->lock);
}

/**
 * here get the new connection coming
 */
static void link_connect_callback(void *user, void *connection)
{
    struct link_manager *lm = (struct link_manager *)user;

    struct link *link = create_link(lm->name, NO_CREATE_CONNECTION);

    link_init(link, lm->name, connection);

    pthread_mutex_lock(&lm->lock);

    list_add(&link->entry, &lm->list);

    pthread_mutex_unlock(&lm->lock);

    /**
     * callback to ipc server
     */
    lm->func(lm, link);

}

static void *link_manager_thread_loop(void *user)
{
    struct link_manager *lm = (struct link_manager *)user;

    lm->is_running = 1;

    while (lm->is_running) {
        int ret = link_wait_for_connect(lm->link, lm, link_connect_callback);
        if (ret < 0) {
            log_e("%s-%s: erorr to wait for connect\n", lm->name, __func__);
            break;
        }
    }

    delete_link_manager(lm);

    return (void *)0;
}


