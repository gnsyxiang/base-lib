/**
 * Copyright (C) 2017 xxx Co., Ltd.
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

UART_HELPER_EX int uart_init(char *port_path);

#ifdef __cplusplus
}
#endif

#endif /* end _UART_HELPER_H_ */

