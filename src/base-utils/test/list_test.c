/**
 * 
 * Release under GPLv2.
 * 
 * @file    list_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/07 2019 14:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/07 2019      create the file
 * 
 *     last modified: 25/07 2019 14:20
 */
#include <stdio.h>

#include "list.h"

static struct list_head g_list_head;

struct msg {
    int cnt;
    struct list_head list;
};

void list_test(void)
{
    INIT_LIST_HEAD(&g_list_head);

#define MSG_CNT     (5)
    struct msg test[MSG_CNT];
    for (int i = 0; i < MSG_CNT; i++) {
        test[i].cnt = i;
        // INIT_LIST_HEAD(&test[i].list);
        list_add_tail(&test[i].list, &g_list_head);
    }

    struct msg *pos, *n;
    list_for_each_entry(pos, &g_list_head, list) {
        if (pos) {
            printf("cnt: %d \n", pos->cnt);
        }
    }

    list_for_each_entry_safe(pos, n, &g_list_head, list) {
        printf("cnt: %d \n", pos->cnt);
        list_del(&pos->list);
    }

}

