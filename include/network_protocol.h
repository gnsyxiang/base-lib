/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    network_protocol.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/12 2017 22:57
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/12 2017      create the file
 * 
 *     last modified: 08/12 2017 22:57
 */
#ifndef _NETWORK_PROTOCOL_H_
#define _NETWORK_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "socket_helper.h"

#ifndef NETWORK_PROTOCOL_GB
#define NETWORK_PROTOCOL_EX extern
#else
#define NETWORK_PROTOCOL_EX
#endif

#define FRONT_SYMBOL	(0xaa)
#define TAIL_SYMBOL		(0x55)

#define SOCKET_READ_TIMEOUT_MS (1000)

#define READ_MESSAGE_LEN (7)

void send_ok(socket_t *sk);
void send_wait(socket_t *sk);
void check_is_package(unsigned char *buf, int ret, handle_message_t handle_message);

#ifdef __cplusplus
}
#endif

#endif /* end _NETWORK_PROTOCOL_H_ */

