/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    link.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 20:00
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 20:00
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <limits.h>

#include "link.h"
#include "list.h"

void link_init(struct link *link, const char *name, void *connection)
{
    int size = strlen(name) + 1 > LINK_NAME_MAX_LEN ? LINK_NAME_MAX_LEN : strlen(name) + 1;

    memcpy(link->name, name, size);

    link->name[size] = 0;
    link->tag[0] = 0;

    link->type = IPC_LINK;
    link->pid = getpid();
    link->connection = connection;
    link->is_connect = 1;
    link->used_count = 1;
    INIT_LIST_HEAD(&link->entry);
}

struct link *create_link(const char *name, enum create_type type)
{
    struct link_ops *link_ops = get_link_ops();
    assert(link_ops);

    struct link *link = (struct link *)malloc(sizeof(struct link));

    link_init(link, name, NULL);

    link->is_connect = 0;

    pthread_mutex_init(&link->lock, NULL);

    switch (type) {
    case CONNECTION_SERVER:
        link->connection = link_ops->create_server(link->name);
        if (link->connection == NULL) {
            delete_link(link);
            return NULL;
        }
        break;

    case CONNECTION_CLIENT:
        link->connection = link_ops->create_client(link->name);
        if (link->connection == NULL) {
            delete_link(link);
            return NULL;
        }
        break;

    case NO_CREATE_CONNECTION:
        break;
    default:
        log_e("%s: error create type: %d\n", __func__, type);
        assert(0);
    }

    return link;
}

void link_disconnect(struct link *link)
{
    assert(link->type == IPC_LINK);

    struct link_ops *link_ops = get_link_ops();
    assert(link_ops);

    pthread_mutex_lock(&link->lock);

    link->is_connect = 0;

    if (link->connection)
        link_ops->delete(link->connection);
    link->connection = NULL;

    pthread_mutex_unlock(&link->lock);
}

void delete_link(struct link *link)
{
    assert(link->type == IPC_LINK);

    struct link_ops *link_ops = get_link_ops();
    assert(link_ops);

    link_disconnect(link);

    pthread_mutex_destroy(&link->lock);
    free(link);
}

void get_link(struct link *link)
{
    assert(link->type == IPC_LINK);

    pthread_mutex_lock(&link->lock);

    link->used_count ++;

    pthread_mutex_unlock(&link->lock);
}

void put_link(struct link *link)
{
    assert(link->type == IPC_LINK);

    pthread_mutex_lock(&link->lock);

    link->used_count--;
    int use = link->used_count;

    pthread_mutex_unlock(&link->lock);

    if (!use) {
        delete_link(link);
    }
}

int link_read(struct link *link, void *buf, int size)
{
    assert(link->type == IPC_LINK);

    struct link_ops *link_ops = get_link_ops();
    assert(link_ops);

    pthread_mutex_lock(&link->lock);

    int ret = -1;
    if (link->is_connect) {
        ret = link_ops->read(link->connection, buf, size);
	}

    pthread_mutex_unlock(&link->lock);

    return ret;
}

int link_write(struct link *link, const void *buf, int size)
{
    assert(link->type == IPC_LINK);

    struct link_ops *link_ops = get_link_ops();
    assert(link_ops);

    pthread_mutex_lock(&link->lock);

    int ret = -1;
    if (link->is_connect) {
        ret = link_ops->write(link->connection, &size, sizeof(int));
		if (ret == sizeof(int)) {
			ret = link_ops->write(link->connection, buf, size);
		}
    }

    pthread_mutex_unlock(&link->lock);

    return ret;
}

int link_connect_timeout(struct link *link, int timeout)
{
    assert(link->type == IPC_LINK);

    struct link_ops *link_ops = get_link_ops();
    assert(link_ops);

    if (link->is_connect)
        return 0;

    pthread_mutex_lock(&link->lock);

    if (link->connection == NULL) {
        link->connection =
                link_ops->create_client(link->name);
        if (link->connection == NULL) {

            pthread_mutex_unlock(&link->lock);
            return -1;
        }
    }

    int ret = link_ops->connect(link->connection, timeout);
    if (ret < 0) {

        pthread_mutex_unlock(&link->lock);
        return ret;
    }

    link->is_connect = 1;
    pthread_mutex_unlock(&link->lock);

    return ret;
}

int link_connect(struct link *link)
{
    return link_connect_timeout(link, INT_MAX);
}

int link_is_connect(struct link *link)
{
    assert(link->type == IPC_LINK);

    return link->is_connect;
}

int link_wait_for_connect(struct link *link, void *user, link_connect_callback_t cb)
{
    assert(link->type == IPC_LINK);

    struct link_ops *link_ops = get_link_ops();
    assert(link_ops);

    if (link->connection == NULL) {
        link->connection = link_ops->create_server(link->name);
        if (link->connection == NULL)
            return -1;
    }

    return link_ops->wait_for_connect(link->connection, user, cb);
}

void set_link_info(struct link *link, const char *name, const char *tag, int pid)
{
    int size = strlen(name) + 1 > LINK_NAME_MAX_LEN ? LINK_NAME_MAX_LEN : strlen(name) + 1;
    memcpy(link->name, name, size);
    link->name[size] = 0;

    size = strlen(tag) + 1 > LINK_NAME_MAX_LEN ? LINK_NAME_MAX_LEN : strlen(tag) + 1;
    memcpy(link->tag, tag, size);
    link->tag[size] = 0;

    link->pid = pid;
}

struct ipc_info get_link_info(struct link *link)
{
    assert(link->type == IPC_LINK);

    struct ipc_info info;

    info.name = link->name;
    info.tag = link->tag;
    info.pid = link->pid;

    return info;
}

int get_link_fd(struct link *link)
{
    assert(link->type == IPC_LINK);

    struct link_ops *link_ops = get_link_ops();
    assert(link_ops);

    return link_ops->get_fd(link->connection);
}

void dump_link_info(struct link *link, int tid)
{
    assert(link->type == IPC_LINK);

    log_d("===========link dump=============\n");

    log_d("name: %s, tag: %s, pid: %d, tid: %d\n",
            link->name, link->tag, link->pid, tid);

    log_d("=================================\n");
}

