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
#include <ctype.h>

#include <log_helper.h>

#include "net-card.h"

#define NETCARD_DEBUG

#define GET_SOCKET(sk)								\
	do {											\
		sk = socket(AF_INET, SOCK_DGRAM, 0);		\
		if (sk < 0) {								\
			log_e("create socket failed");			\
			ret = -1;								\
		}											\
	} while(0)

#define DEF_IFR_VAR()								\
    struct ifreq ifr;								\
	memset(&ifr, '\0', sizeof(struct ifreq));		\
    strcpy(ifr.ifr_name, netcard_name);

static bl_int32_t get_netcard_info(const bl_int8_t *netcard_name, bl_int8_t *ip, bl_int32_t cmd)
{
	bl_int32_t sk = -1;
	bl_int32_t ret = 0;

	GET_SOCKET(sk);

	DEF_IFR_VAR();

	if (cmd == SIOCGIFHWADDR) {
		if (ioctl(sk, SIOCGIFHWADDR, &ifr) == 0) {
			bl_uint8_t buf[6] = {0};

			memcpy(buf, ifr.ifr_hwaddr.sa_data, sizeof(buf));
			sprintf(ip, "%x:%x:%x:%x:%x:%x", 
					buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
		}
	} else {
		if (ioctl(sk, cmd, &ifr) == 0) {
			bl_uint64_t buf = *(bl_uint64_t *)&ifr.ifr_broadaddr.sa_data[2];
			sprintf(ip, "%s", inet_ntoa(*(struct in_addr*)&buf));
		}
	}

    close(sk);
	return ret;
}


bl_int32_t get_netcard_num(bl_int8_t (*net_card_info)[64])
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

bl_int32_t get_netcard_ip(const bl_int8_t *netcard_name, bl_int8_t *ip)
{
	return get_netcard_info(netcard_name, ip, SIOCGIFADDR);
}

bl_int32_t get_netcard_broadip(const bl_int8_t *netcard_name, bl_int8_t *broadip)
{
	return get_netcard_info(netcard_name, broadip, SIOCGIFBRDADDR);
}

bl_int32_t get_netcard_netmast(const bl_int8_t *netcard_name, bl_int8_t *netmask)
{
	return get_netcard_info(netcard_name, netmask, SIOCGIFNETMASK);
}

bl_int32_t get_netcard_mac(const bl_int8_t *netcard_name, bl_int8_t *mac)
{
	return get_netcard_info(netcard_name, mac, SIOCGIFHWADDR);
}

#define IOCTL_SET(cmd)								\
	do {											\
		if (ioctl(sk, cmd, &ifr) < 0) {				\
			log_e("ioctl %d faild", cmd);			\
			ret = -1;								\
		}											\
	} while (0)

bl_int32_t set_netcard_ip(const bl_int8_t *netcard_name, const bl_int8_t *ip)
{
	bl_int32_t sk = -1;
	bl_int32_t ret = 0;

	GET_SOCKET(sk);

	DEF_IFR_VAR();

	struct sockaddr_in sin_set_ip;
	sin_set_ip.sin_family = AF_INET;  
	sin_set_ip.sin_addr.s_addr = inet_addr(ip);  

	memcpy(&ifr.ifr_addr, &sin_set_ip, sizeof(sin_set_ip));
	IOCTL_SET(SIOCSIFADDR);

	//设置激活标志  
	ifr.ifr_flags |= IFF_UP | IFF_RUNNING;  
	IOCTL_SET(SIOCSIFFLAGS);

    close(sk);

	return ret;
}

bl_int32_t set_netcard_netmast(const bl_int8_t *netcard_name, const bl_int8_t *netmask)
{
	bl_int32_t sk = -1;
	bl_int32_t ret = 0;

	GET_SOCKET(sk);

	DEF_IFR_VAR();

	struct sockaddr_in *sin_net_mask;
	sin_net_mask = (struct sockaddr_in *)&(ifr.ifr_addr);  

	sin_net_mask->sin_family = AF_INET;  
	inet_pton(AF_INET, netmask, &sin_net_mask->sin_addr);  

	IOCTL_SET(SIOCSIFNETMASK);

    close(sk);

	return ret;
}

bl_int32_t get_netcard_gateway(bl_int8_t *gateway)
{  
	bl_int8_t buf[512];  
	bl_int8_t *tmp;  
	bl_int32_t ret = 0;

	FILE *fp = popen("ip route", "r");  
	if(NULL == fp) {  
		log_e("popen faild");
		ret = -1;
	}  

	while (fgets(buf, sizeof(buf), fp) != NULL) {  
		tmp = buf;  
		while (*tmp && isspace(*tmp))  
			++tmp;  
		if (strncmp(tmp, "default", strlen("default")) == 0)  
			break;  
	}

	sscanf(buf, "%*s%*s%s", gateway);         
	pclose(fp);  

	return ret;
}

void disp_netcard_info(const bl_int8_t *netcard_name, 
		bl_int8_t *ip, bl_int8_t *mac, bl_int8_t *broadip, 
		bl_int8_t *netmask, bl_int8_t *gateway)
{
	get_netcard_mac(netcard_name, mac);
	get_netcard_ip(netcard_name, ip);
	get_netcard_broadip(netcard_name, broadip);
	get_netcard_netmast(netcard_name, netmask);

	get_netcard_gateway(gateway);
}

