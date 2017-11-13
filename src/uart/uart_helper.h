/****************************************************************
 * Filename: uart_helper.h
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 13/11 2017 22:31
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	13/11 2017
 ****************************************************************/

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

