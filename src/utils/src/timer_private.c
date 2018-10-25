/**
 * 
 * Release under GPLv2.
 * 
 * @file    timer_private.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/10 2018 14:39
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/10 2018      create the file
 * 
 *     last modified: 25/10 2018 14:39
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include "list.h"
#include "log_helper.h"
#include "thread_helper.h"
#include "timer_private.h"

typedef struct timer_list_tag {
    struct list_head list;
    pthread_mutex_t mutex;

    int init_flag;
    int timer_list_loop_is_running;

    int pfd[2];
}timer_list_t;

static timer_list_t timer_list;

#define timer_list_lock()   pthread_mutex_lock(&timer_list.mutex);
#define timer_list_unlock() pthread_mutex_unlock(&timer_list.mutex);

static void timer_list_notify(void)
{
    char ch;
    write(timer_list.pfd[1], &ch, sizeof(ch));
}

void timer_list_add(timer_tag *timer)
{
    timer_list_lock();

    list_add(&timer->entry, &timer_list.list);

    timer_list_unlock();

    timer_list_notify();
}

int timer_list_del(timer_tag *timer, timer_cb_t timer_cb)
{
    int ret = 0;
    timer_tag *timer_tmp, *tmp;

    timer_list_lock();

    list_for_each_entry_safe(timer_tmp, tmp, &timer_list.list, entry) {
        if (timer_tmp == timer && timer->timer_cb == timer_cb) {
            list_del_init(&timer->entry);

            ret = 1;
            break;
        }
    }

    timer_list_unlock();

    return ret;
}

static void *timer_list_loop(void *args)
{
    timer_tag *tmp, *timer;
    int max_fd;
    int ret;
    char ch;

    while (timer_list.timer_list_loop_is_running) {
        fd_set read_fs;

        FD_ZERO(&read_fs);
        FD_SET(timer_list.pfd[0], &read_fs);

        max_fd = timer_list.pfd[0];

        timer_list_lock();

        list_for_each_entry(timer, &timer_list.list, entry) {
            max_fd = max_fd > timer->fd ? max_fd : timer->fd;
            FD_SET(timer->fd, &read_fs);
        }

        timer_list_unlock();

        ret = select(max_fd + 1, &read_fs, NULL, NULL, NULL);
        if (ret < 0) {
            log_e("select error");
            continue;
        }

        if (FD_ISSET(timer_list.pfd[0], &read_fs)) {
            read(timer_list.pfd[0], &ch, sizeof(ch));
        }

        list_for_each_entry_safe(timer, tmp, &timer_list.list, entry) {
            if (FD_ISSET(timer->fd, &read_fs)) {
                list_del_init(&timer->entry);

                create_a_attached_thread(NULL, (do_timer_start_cb_t)args, timer);
            }
        }

        timer_list_unlock();
    }

    return NULL;
}

void timer_list_init(void *args)
{
    if (timer_list.init_flag)
        return;

    timer_list.init_flag = 1;
    timer_list.timer_list_loop_is_running = 1;

    pipe(timer_list.pfd);

    INIT_LIST_HEAD(&timer_list.list);
    pthread_mutex_init(&timer_list.mutex, NULL);

    create_a_attached_thread(NULL, timer_list_loop, args);
}

void timer_list_clean(void)
{
    timer_list.timer_list_loop_is_running = 0;
}

