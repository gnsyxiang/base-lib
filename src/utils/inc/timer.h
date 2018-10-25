/**
 *
 * Release under GPLv2.
 * 
 * @file    timer.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/10 2018 11:10
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/10 2018      create the file
 * 
 *     last modified: 25/10 2018 11:10
 */
#ifndef __UTILS_TIMER_H_
#define __UTILS_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UTILS_TIMER_GB
#define UTILS_TIMER_EX extern
#else
#define UTILS_TIMER_EX
#endif

typedef void (*timer_cb_t)(void *args);

UTILS_TIMER_EX void timer_init(void);
UTILS_TIMER_EX void timer_clean(void);

UTILS_TIMER_EX int timer_start(timer_cb_t timer_cb, 
        unsigned int timeout_ms, void *args);

UTILS_TIMER_EX int timer_cancel(int timer_handler, timer_cb_t timer_cb);

#ifdef __cplusplus
}
#endif

#endif /* __UTILS_TIMER_H_ */

