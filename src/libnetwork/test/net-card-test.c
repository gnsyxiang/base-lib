/**
 * 
 * Release under GPLv2.
 * 
 * @file    net-card-test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/05 2018 18:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/05 2018      create the file
 * 
 *     last modified: 29/05 2018 18:06
 */
#include <stdio.h>

#include <utils/log_helper.h>
#include <utils/parse_cmd.h>

#include "net-card.h"

void net_card_test(void)
{
	bl_int8_t netcard_name[2][64] = {0};
	bl_int8_t ip[64] = {0};
	bl_int8_t mac[64] = {0};
	bl_int8_t broadip[64] = {0};
	bl_int8_t netmask[64] = {0};
	bl_int8_t gateway[64] = {0};

	int cnt = get_netcard_num(netcard_name);

	for (int i = 0; i < cnt; i++) {
		log_i("name: %s ", netcard_name[i]);

		disp_netcard_info(netcard_name[i], ip, mac, broadip, netmask, gateway);

		log_i("mac: %s ", mac);
		log_i("ip: %s ", ip);
		log_i("broadip: %s ", broadip);
		log_i("netmask: %s ", netmask);
		log_i("gateway: %s ", gateway);
	}
}

static void net_card_test_init(void)
{
	printf("1. net card test \n");

	handle_test_cmd_t net_card_test_cmd[] = {
		{"1", net_card_test},
	};

	register_test_cmd(net_card_test_cmd, ARRAY_NUM(net_card_test_cmd));
}
DECLARE_INIT(net_card_test_init);

