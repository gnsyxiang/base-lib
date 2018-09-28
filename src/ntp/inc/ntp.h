/**
 *
 * Release under GPLv2.
 * 
 * @file    ntp.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    27/09 2018 10:44
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        27/09 2018      create the file
 * 
 *     last modified: 27/09 2018 10:44
 */
#ifndef __BASE_LIB_NTP_H_
#define __BASE_LIB_NTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_NTP_GB
#define BASE_LIB_NTP_EX extern
#else
#define BASE_LIB_NTP_EX
#endif

BASE_LIB_NTP_EX void ntp_init(const char *hostname, unsigned int sync_time_s);

BASE_LIB_NTP_EX void net_sync_time(void);

BASE_LIB_NTP_EX void ntp_clean(void);

#ifdef __cplusplus
}
#endif

#endif

