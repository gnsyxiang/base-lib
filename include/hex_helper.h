/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    hex_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/12 2017 22:24
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/12 2017      create the file
 * 
 *     last modified: 08/12 2017 22:24
 */
#ifndef _HEX_HELPER_H_
#define _HEX_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HEX_HELPER_GB
#define HEX_HELPER_EX extern
#else
#define HEX_HELPER_EX
#endif

HEX_HELPER_EX void print_hex(char *buf, int len);


#ifdef __cplusplus
}
#endif

#endif /* end _HEX_HELPER_H_ */

