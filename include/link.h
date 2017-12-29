/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    link.h
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
#ifndef IPC_LINK_H
#define IPC_LINK_H

#include "socket_helper.h"
#include "list.h"
#include "ipc_private.h"

#define LINK_NAME_MAX_LEN   32

struct link {
    int type;
    struct list_head entry;

    int pid;
    char name[LINK_NAME_MAX_LEN];
    char tag[LINK_NAME_MAX_LEN];

    void *connection;
    int is_connect;
    int used_count;
    pthread_mutex_t lock;
};

typedef void (*link_connect_callback_t)(
        void *user, void *connection);

enum create_type {
    CONNECTION_CLIENT,
    CONNECTION_SERVER,
    NO_CREATE_CONNECTION,
};

void link_init(struct link *link,
        const char *name, void *connection);
struct link *create_link(const char *name, enum create_type type);
void delete_link(struct link *link);

void get_link(struct link *link);
void put_link(struct link *link);

int link_read(struct link *link, void *buf, int size);
int link_write(struct link *link, const void *buf, int size);

int link_connect_timeout(struct link *link, int timeout);
int link_connect(struct link *link);

int link_wait_for_connect(struct link *link,
        void *user, link_connect_callback_t cb);

int link_is_connect(struct link *link);
void link_disconnect(struct link *link);

void set_link_info(struct link *link,
        const char *name, const char *tag, int pid);
struct ipc_info get_link_info(struct link *link);

int get_link_fd(struct link *link);

void dump_link_info(struct link *link, int tid);

#endif
