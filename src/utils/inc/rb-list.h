/**
 *
 * Release under GPLv2.
 * 
 * @file    rb-list.h
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
#ifndef __UTILS_RB_LIST_H_
#define __UTILS_RB_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RB_LIST_GB
#define RB_LIST_EX extern
#else
#define RB_LIST_EX
#endif

struct rb_list_head;

struct rb_list_head *rb_list_init(void);
void rb_list_clean(struct rb_list_head *rb_list_head);

int rb_list_put(struct rb_list_head *rb_list_head, char *buf, int len);
int rb_list_get(struct rb_list_head *rb_list_head, char *buf, int *len);

#ifdef __cplusplus
}
#endif

#endif

