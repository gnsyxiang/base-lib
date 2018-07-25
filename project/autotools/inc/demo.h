/**
 * 
 * Release under GPLv2.
 * 
 * @file    demo.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/07 2018 14:21
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/07 2018      create the file
 * 
 *     last modified: 25/07 2018 14:21
 */
#ifndef __DEMO_H_
#define __DEMO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LIB_SO_NMAE		"libdemo.so"

void demo_init(void);
void demo_clean(void);

#ifdef __cplusplus
}
#endif

#endif

