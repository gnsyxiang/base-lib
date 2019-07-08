/**
 * 
 * Release under GPLv2.
 * 
 * @file    log_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/07 2019 17:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/07 2019      create the file
 * 
 *     last modified: 17/07 2019 17:16
 */
#include <stdio.h>

#include "log_wrapper.h"

#define LOG_TEST_TAG "log_test"

void log_test(void)
{
    loga(LOG_TEST_TAG, "%s world, %d, %s \n", "hello", 7, "haha");
    loge(LOG_TEST_TAG, "%s world\n", "hello");
    logw(LOG_TEST_TAG, "%s world\n", "hello");
    logi(LOG_TEST_TAG, "%s world\n", "hello");
    logd(LOG_TEST_TAG, "%s world\n", "hello");
    logv(LOG_TEST_TAG, "%s world\n", "hello");
}

