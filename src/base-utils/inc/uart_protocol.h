/**
 *
 * Release under GPLv2.
 * 
 * @file    uart_protocol.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/08 2019 17:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/08 2019      create the file
 * 
 *     last modified: 08/08 2019 17:06
 */
#ifndef __BASE_LIB_UTILS_INC_UART_PROTOCOL_H_
#define __BASE_LIB_UTILS_INC_UART_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_UTILS_SRC_UART_PROTOCOL_GB
#define BASE_LIB_UTILS_SRC_UART_PROTOCOL_EX extern
#else
#define BASE_LIB_UTILS_SRC_UART_PROTOCOL_EX
#endif

#define UNPACK __attribute__((packed))

typedef struct frame_tag {
    int cmd;
    unsigned short len;
    char *data;
} UNPACK frame_t;
#define FRAME_LEN (sizeof(frame_t))

typedef void (*uart_protocol_read_cb_t)(frame_t *buf);

typedef struct uart_protocol_config_tag {
    uart_protocol_read_cb_t uart_protocol_read_cb;

    int heatbeat_flag;
    int heatbeat_timeout_ms;
} uart_protocol_config_t;

int uart_protocol_init(uart_protocol_config_t *uart_protocol_config);
void uart_protocol_final(void);

int uart_protocol_write(void *frame);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_UART_PROTOCOL_H_ */

