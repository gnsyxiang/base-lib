/**
 *
 * Release under GPLv2.
 * 
 * @file    timer_private.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/10 2018 15:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/10 2018      create the file
 * 
 *     last modified: 25/10 2018 15:13
 */
#ifndef __UTILS_TIMER_PRIVATE_H_
#define __UTILS_TIMER_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "list.h"
#include "timer.h"

typedef struct timer {
    int fd;
    struct list_head entry;

    timer_cb_t timer_cb;
    void *args;
}timer_tag;

#define TIMER_T_LEN     (sizeof(timer_tag))

typedef void *(*do_timer_start_cb_t)(void *args);

void timer_list_init(void *args);
void timer_list_clean(void);

void timer_list_add(timer_tag *timer);
int timer_list_del(timer_tag *timer, timer_cb_t timer_cb);


#ifdef __cplusplus
}
#endif

#endif /* __UTILS_TIMER_PRIVATE_H_ */

