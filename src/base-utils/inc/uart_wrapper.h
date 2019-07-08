/**
 *
 * Release under GPLv2.
 * 
 * @file    uart_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/08 2019 17:07
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/08 2019      create the file
 * 
 *     last modified: 08/08 2019 17:07
 */
#ifndef __BASE_LIB_UTILS_INC_UART_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_UART_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_UTILS_SRC_UART_WRAPPER_GB
#define BASE_LIB_UTILS_SRC_UART_WRAPPER_EX extern
#else
#define BASE_LIB_UTILS_SRC_UART_WRAPPER_EX
#endif

typedef enum {
  UART_NUM_1,
  UART_NUM_2,
  UART_NUM_3,
  UART_NUM_MAX,
} uart_num_t;

typedef enum {
  UART_SPEED_9600,
  UART_SPEED_115200,
  UART_SPEED_MAX,
} uart_speed_t;

typedef void (*uart_read_cb_t)(void *buf, size_t len);

typedef struct uart_config_tag {
  uart_num_t uart_num;
  uart_speed_t speed;
  uart_read_cb_t uart_read_cb;
  size_t select_timeout_ms;
} uart_config_t;
#define UART_CONFIG_LEN (sizeof(uart_config_t))

void *uart_init(uart_config_t *uart_config);
int uart_final(void *handle);

int uart_write(void *handle, void *buf, size_t len);
int uart_write_headbeat(void *handle, void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_UART_WRAPPER_H_ */

