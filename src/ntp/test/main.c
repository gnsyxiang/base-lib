/**
 * 
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    27/09 2018 14:28
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        27/09 2018      create the file
 * 
 *     last modified: 27/09 2018 14:28
 */
#include <stdio.h>
#include <unistd.h>

#include "ntp.h"

int main(int argc, char const* argv[])
{
    ntp_init("ntp1.aliyun.com", 10);

    while (1) {
        sleep(2);
        net_sync_time();
    }

    ntp_clean();

    return 0;
}
