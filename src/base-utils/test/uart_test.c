/**
 * 
 * Release under GPLv2.
 * 
 * @file    uart_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/08 2019 14:18
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/08 2019      create the file
 * 
 *     last modified: 09/08 2019 14:18
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "uart_protocol.h"
#include "thread_wrapper.h"

static void _uart_test_write(void)
{
    int cnt = 0;
    frame_t frame;
    while (cnt++ < 3) {
        frame.cmd = 0x01;
        frame.len = 10;
        frame.data = calloc(1, 10);
        if (!frame.data) {
            printf("calloc faild \n");
        }
        sleep(1);
        printf("cnt: %d \n", cnt);
        uart_protocol_write(&frame);
    }

    sleep(5);
}

void uart_protocol_read_cb(frame_t *frame)
{
    printf("---haha---cmd: %08x \n", frame->cmd);

    switch (frame->cmd) {
        case 1:
            printf("---1---len: %04x \n", frame->len);
            break;
        case 2:
            printf("---2---len: %04x \n", frame->len);
            break;
        default:
            break;
    }

    free(frame->data);
    free(frame);
}

void uart_test(void)
{
    uart_protocol_config_t uart_protocol_config;
    uart_protocol_config.uart_protocol_read_cb = uart_protocol_read_cb;
    uart_protocol_config.heatbeat_flag         = 1;
    uart_protocol_config.heatbeat_timeout_ms   = 1000;

    uart_protocol_init(&uart_protocol_config);
    _uart_test_write();
    uart_protocol_final();
}

