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
    rb_list_init();

    char *buf = "12345";
    rb_list_put(buf, 5);

    char tmp[10] = {0};
    int len = 0;
    rb_list_get(tmp, &len);
    log_i("tmp: %s, len: %d", tmp, len);

    rb_list_clean();

    return 0;
}

