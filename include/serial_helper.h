/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    serial_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/12 2017 22:51
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/12 2017      create the file
 * 
 *     last modified: 12/12 2017 22:51
 */
#ifndef _SERIAL_HELPER_H_
#define _SERIAL_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SERIAL_HELPER_GB
#define SERIAL_HELPER_EX extern
#else
#define SERIAL_HELPER_EX
#endif

SERIAL_HELPER_EX int serial_open(char *port_path);

#ifdef __cplusplus
}
#endif

#endif /* end _SERIAL_HELPER_H_ */

