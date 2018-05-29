/**
 * 
 * Release under GPLv2.
 * 
 * @file    net-card.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/05 2018 13:58
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/05 2018      create the file
 * 
 *     last modified: 29/05 2018 13:58
 */
#ifndef __NET_CARD_H_
#define __NET_CARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <typedef_helper.h>

#define NET_DEV_NAME	"/proc/net/dev"

bl_int32_t get_netcard_info(bl_int8_t (*net_card_info)[64]);
void disp_netcard_info(const bl_int8_t *netcard_name, 
		bl_int8_t *ip, bl_int8_t *mac, bl_int8_t *broadip, bl_int8_t *netmask);

#ifdef __cplusplus
}
#endif

#endif

