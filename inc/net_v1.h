/**
 *
 * Release under GPLv2.
 * 
 * @file    net_v1.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/02 2018 13:35
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/02 2018      create the file
 * 
 *     last modified: 02/02 2018 13:35
 */
#ifndef __BASE_LIB_NET_V1_H_
#define __BASE_LIB_NET_V1_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "socket_helper.h"

#ifndef NET_V1_GB
#define NET_V1_EX extern
#else
#define NET_V1_EX
#endif

NET_V1_EX void net_v1_client_init(socket_cb_t socket_client_cb);
NET_V1_EX void net_v1_server_init(socket_cb_t socket_server_cb);

#ifdef __cplusplus
}
#endif

#endif /* end __BASE_LIB_NET_V1_H_ */

