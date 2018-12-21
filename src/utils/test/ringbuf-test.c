/**
 * 
 * Release under GPLv2.
 * 
 * @file    ringbuf-test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/12 2018 14:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/12 2018      create the file
 * 
 *     last modified: 21/12 2018 14:06
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "thread_helper.h"
#include "ringbuf.h"
#include "log_helper.h"

static  prb_t test_rf;

void *test_rb_loop(void *args)
{
    char buf[3];
    while (1) {
        memset(buf, '\0', sizeof(buf));
        rb_out(test_rf, buf, 2);

        log_i("----1--->buf: %s ", buf);
        sleep(3);
    }

    return NULL;
}

int rb_test(void)
{
    test_rf = rb_init(5);
    if (!test_rf) {
        log_e("video ringbuf init faild");
        return -1;
    }

    create_a_attached_thread(NULL, test_rb_loop, NULL);

    int len;
    char i = 'a';

    while (1) {
        rb_in(test_rf, &i, 1);
        len = rb_remain_size(test_rf);
        log_i("len: %d", len);
        sleep(1);

        if (i++ == 'x')
            i = 'a';
    }

    rb_clean(test_rf);
    
    return 0;
}
