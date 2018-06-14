/**
 *
 * Release under GPLv2.
 * 
 * @file    time_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/12 2017 16:50
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/12 2017      create the file
 * 
 *     last modified: 20/12 2017 16:50
 */
#include <stdio.h>

#include "time_helper.h"
#include "misc_helper.h"
#include "parse_cmd.h"

static void random_test(void)
{
    for (int i = 0; i < 100; i++) {
        printf("%d ",  random_num(100));
    }
}

static void time_test(void)
{
    struct timespec ts;
    struct tm now_time;
    char time_buf[256];

    printf("-1- sec: %ld \n", time(NULL));

    ts = cur_delay_ms(2000);
    printf("-2- sec: %ld \n", ts.tv_sec);
    printf("nsec: %ld \n", ts.tv_nsec);

    get_tm_time(&now_time);
    format_time(time_buf);

    printf("time: %s \n", time_buf);

	printf("time_ms: %ld \n", get_sys_time_ms());

	random_test();
}

static void time_test_init(void)
{
	printf(" 4. time test \n");

	handle_test_cmd_t time_test_cmd[] = {
		{"4", time_test}
	};

	register_test_cmd(time_test_cmd, ARRAY_NUM(time_test_cmd));
}
DECLARE_INIT(time_test_init);

