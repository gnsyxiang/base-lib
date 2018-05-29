/**
 * 
 * Release under GPLv2.
 * 
 * @file    net-card.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/05 2018 13:59
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/05 2018      create the file
 * 
 *     last modified: 29/05 2018 13:59
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>    

#include <log_helper.h>

#include "network-card.h"

bl_int32_t get_netcard_info(bl_int8_t (*net_card_info)[64])
{
	bl_int32_t num = 0;

	FILE* fp = fopen(NET_DEV_NAME, "r");
	if (!fp) {
		log_e("fopen %s failed", NET_DEV_NAME);
		return num;
	}

	bl_int8_t buf[512];

	fgets(buf, sizeof(buf), fp);   
	fgets(buf, sizeof(buf), fp);

	while (fgets(buf, sizeof(buf), fp)) {
		bl_int8_t name[64] = {0};
		sscanf(buf, "%s", name);
		bl_int32_t len = strlen(name);

		if (len <= 0)
			continue;
		if (name[len - 1] == ':') 
			name[len - 1] = '\0';
		if (strcmp(name, "lo") == 0)
			continue;

		strcpy((char *)net_card_info++, name);

		num++;
	}

	fclose(fp);
	fp = NULL;
	return num;
}

void disp_netcard_info(const bl_int8_t *netcard_name, 
		bl_int8_t *ip, bl_int8_t *pmac, bl_int8_t *broadip, bl_int8_t *netmask)
{
    int sk = socket(AF_INET, SOCK_DGRAM, 0);
    if (sk < 0) {
        log_e("create socket failed");
        return;
    }

    struct ifreq ifr = {0};
    bl_uint8_t mac[6] = {0};
    bl_uint64_t ip_tmp = 0;

    strcpy(ifr.ifr_name, netcard_name);

    if (ioctl(sk, SIOCGIFHWADDR, &ifr) == 0) {
		memcpy(mac, ifr.ifr_hwaddr.sa_data, sizeof(mac));
		sprintf(pmac, "%x-%x-%x-%x-%x-%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }

    if (ioctl(sk, SIOCGIFADDR, &ifr) == 0) {
        ip_tmp = *(bl_uint64_t *)&ifr.ifr_broadaddr.sa_data[2];
		sprintf(ip, "%s", inet_ntoa(*(struct in_addr*)&ip_tmp));
    }

    if (ioctl(sk, SIOCGIFBRDADDR, &ifr) == 0) {
        ip_tmp = *(bl_uint64_t *)&ifr.ifr_broadaddr.sa_data[2];
		sprintf(broadip, "%s", inet_ntoa(*(struct in_addr*)&ip_tmp));
    }

    if (ioctl(sk, SIOCGIFNETMASK, &ifr) == 0) {
        ip_tmp = *(bl_uint64_t *)&ifr.ifr_netmask.sa_data[2];
		sprintf(netmask, "%s", inet_ntoa(*(struct in_addr*)&ip_tmp));
    }

    close(sk);
}

