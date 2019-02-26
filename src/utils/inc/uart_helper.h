/**
 *
 * Release under GPLv2.
 * 
 * @file    uart_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/12 2017 22:51
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/12 2017      create the file
 * 
 *     last modified: 12/12 2017 22:51
 */
#ifndef _UART_HELPER_H_
#define _UART_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UART_HELPER_GB
#define UART_HELPER_EX extern
#else
#define UART_HELPER_EX
#endif

typedef struct _uart_param_tag {
	int fd;
	int baud_rate;
	int parity_check;
	int bsize;
	int stop;
} uart_param_t;

UART_HELPER_EX int uart_open(const char *port_path);
UART_HELPER_EX void uart_close(int fd);
UART_HELPER_EX int uart_init(int fd, int baude, int c_flow, int bits, char parity, int stop);
UART_HELPER_EX ssize_t uart_read(int fd, char *buf, size_t cnt);
UART_HELPER_EX ssize_t uart_write(int fd, const char *buf, size_t cnt);

#ifdef __cplusplus
}
#endif

#endif /* end _UART_HELPER_H_ */

