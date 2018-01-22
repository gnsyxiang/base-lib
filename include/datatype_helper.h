/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    datatype_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/12 2017 17:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/12 2017      create the file
 * 
 *     last modified: 07/12 2017 17:08
 */
#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DATATYPE_GB
#define DATATYPE_EX extern
#else
#define DATATYPE_EX
#endif

DATATYPE_EX void check_datatype_mem_len(void);


#ifdef __cplusplus
}
#endif

#endif /* end _DATATYPE_H_ */

