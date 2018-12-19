/**
 * 
 * Release under GPLv2.
 * 
 * @file    rb_list_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/12 2018 19:26
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/12 2018      create the file
 * 
 *     last modified: 17/12 2018 19:26
 */
#include <stdio.h>

#include "log_helper.h"
#include "rb-list.h"

int rb_list_test(void)
{
    struct rb_list_head *rb_list_head = rb_list_init();

    for (int i = 0; i < 5; i++) {
        rb_list_put(rb_list_head, (char *)&i, 1);
    }

    int len = 0;
    for (int i = 0; i < 5; i++) {
        rb_list_get(rb_list_head, (char *)&i, &len);
        log_i("tmp: %d, len: %d", i, len);
    }

    rb_list_clean(rb_list_head);

    return 0;
}

