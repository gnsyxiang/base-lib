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

#include "network-card.h"

int main(int argc, char* argv[])
{
	bl_int8_t net_card_info[2][64] = {0};
	bl_int8_t ip[64] = {0};
	bl_int8_t mac[64] = {0};
	bl_int8_t broadip[64] = {0};
	bl_int8_t netmask[64] = {0};

	int cnt = get_netcard_info(net_card_info);

	for (int i = 0; i < cnt; i++) {
		printf("name: %s \n", net_card_info[i]);

		disp_netcard_info(net_card_info[i], ip, mac, broadip, netmask);

		printf("ip: %s \n", ip);
		printf("mac: %s \n", mac);
		printf("broadip: %s \n", broadip);
	}

	return 0;
}
