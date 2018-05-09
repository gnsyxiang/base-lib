/****************************************************************
 * Filename: ringbuf-test.c
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 20/09 2017 18:21
 * Description:
 * 
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	20/09 2017
 ****************************************************************/
#include <stdio.h>
#include <unistd.h>

#include "ringbuf.h"
#include "parse_cmd.h"
#include "misc_helper.h"
#include "pthread_helper.h"
#include "log_helper.h"

static pringbuf_t pringbuf = NULL;

static void *consumers_cb(void *arg)
{
    char buf[3];

    while (1) {
        usleep(800 * 1000);

        /* ringbuf_out(pringbuf, &buf, 1); */
        /* printf("buf: %d \n", buf[0]); */

        ringbuf_out(pringbuf, &buf, 3);
    }

    return NULL;
}

static void *producers_cb(void *arg)
{
    char buf;
    int i = 0;

    while (1) {
        if (i == 20) {
            break;
        }
        usleep(200 * 1000);
        buf = 'a' + i;
        printf("buf: ----------------------%c \n", buf);
        ringbuf_in(pringbuf, &buf, 1);
        i++;
    }

    return NULL;
}

static void ringbuf_print(pringbuf_t pringbuf)
{
    FOR_ZERO_TO_LEN(pringbuf->size - pringbuf->remain_size) {
        printf("%c ", pringbuf->buf[i]);
    }
    printf("\n");
}

static void ringbuf_test(void)
{
	log_i("ringbuf start");

    pringbuf = ringbuf_init(10);
    if (!pringbuf) {
        printf("ringbuf init faild \n");
    }

    create_a_attached_thread(NULL, consumers_cb, NULL);
    create_a_attached_thread(NULL, producers_cb, NULL);

    while (1) {
        usleep(100 * 1000);
        printf("size: %d, remain_size: %d \n", pringbuf->size, pringbuf->remain_size);
        printf("is empty: %d \n", ringbuf_is_empty(pringbuf));
        printf("is full: %d \n", ringbuf_is_full(pringbuf));
        ringbuf_print(pringbuf);
    }

    ringbuf_destroy(pringbuf);
}

static void ringbuf_test_init(void)
{
	handle_test_cmd_t ringbuf_test_cmd[] = {
		{"15", ringbuf_test},
	};

	register_test_cmd(ringbuf_test_cmd, ARRAY_NUM(ringbuf_test_cmd));
}
DECLARE_INIT(ringbuf_test_init);

