/**
 * 
 * Release under GPLv2.
 * 
 * @file    timer.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/10 2018 11:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/10 2018      create the file
 * 
 *     last modified: 25/10 2018 11:32
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/timerfd.h>

#include "timer_private.h"
#include "log_helper.h"

int timer_start(timer_cb_t timer_cb, unsigned int timeout_ms, void *args)
{
    int fd = timerfd_create(CLOCK_BOOTTIME, TFD_NONBLOCK);
    if (fd < 0) {
        log_e("fail to timerfd_create");
        return fd;
    }

    timer_tag *timer = (timer_tag *)malloc(TIMER_T_LEN);
    timer->fd       = fd;
    timer->timer_cb = timer_cb;
    timer->args     = args;

    INIT_LIST_HEAD(&timer->entry);

    struct itimerspec it;

    it.it_interval.tv_sec   = 0;
    it.it_interval.tv_nsec  = 0;
    it.it_value.tv_sec      = timeout_ms / 1000;
    it.it_value.tv_nsec     = (timeout_ms % 1000) * 1000000;

    int ret = timerfd_settime(fd, 0, &it, NULL);
    if (ret < 0) {
        log_e("fail to timerfd_settime");
        return ret;
    }

    timer_list_add(timer);

    return (int)timer;
}

int timer_cancel(int timer_handler, timer_cb_t timer_cb)
{
    timer_tag *timer = (timer_tag *)timer_handler;

    int ret = timer_list_del(timer, timer_cb);
    if (ret) {
        close(timer->fd);
        free(timer);
    }

    return ret;
}

void timer_clean(void)
{
    timer_list_clean();
}

static void *do_timer_start_cb(void *args)
{
    timer_tag *timer = (timer_tag *)args;
    timer_cb_t timer_cb = timer->timer_cb;

    timer_cb(timer->args);

    close(timer->fd);
    free(timer);

    return NULL;
}

void timer_init(void)
{
    timer_list_init(do_timer_start_cb);
}

