/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    network_protocol_server.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/12 2017 00:09
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/12 2017      create the file
 * 
 *     last modified: 17/12 2017 00:09
 */
#ifndef _NETWORK_PROTOCOL_SERVER_H_
#define _NETWORK_PROTOCOL_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NETWORK_PROTOCOL_SERVER_GB
#define NETWORK_PROTOCOL_SERVER_EX extern
#else
#define NETWORK_PROTOCOL_SERVER_EX
#endif

int get_server_read_running_flag(void);
void server_send_message(socket_t *sk, unsigned char *buf, int len);
socket_t *network_protocol_server_init(handle_message_t handle_read_message, int read_timeout_ms);


#ifdef __cplusplus
}
#endif

#endif /* end _NETWORK_PROTOCOL_SERVER_H_ */

