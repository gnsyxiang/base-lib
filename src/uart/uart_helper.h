/**
 * 
 * Release under GPLv2.
 * 
 * @file    uart_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/06 2018 10:01
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/06 2018      create the file
 * 
 *     last modified: 14/06 2018 10:01
 */
#ifndef __UART_HELPER_H_
#define __UART_HELPER_H_

#ifdef __cplusplus
export "C" {
#endif

#ifndef UART_HELPER_GB
#define UART_HELPER_EX extern
#else
#define UART_HELPER_EX
#endif

UART_HELPER_EX int uart_init(int fd, int baud, char parity, int bsize, int stop);

#ifdef UART_DEBUG
UART_HELPER_EX void show_termios(unsigned int fd, const struct termios *s);
#endif

#ifdef __cplusplus
}
#endif

#endif /* end __UART_HELPER_H_ */

