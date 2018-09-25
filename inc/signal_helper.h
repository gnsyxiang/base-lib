/**
 *
 * Release under GPLv2.
 * 
 * @file    signal_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/03 2018 16:10
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/03 2018      create the file
 * 
 *     last modified: 02/03 2018 16:10
 */
#ifndef __BASE_LIB_SIGNAL_HELPER_H_
#define __BASE_LIB_SIGNAL_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SIGNAL_HELPER_GB
#define SIGNAL_HELPER_EX extern
#else
#define SIGNAL_HELPER_EX
#endif

SIGNAL_HELPER_EX void register_linux_signal_hanler(const char *app_name);

#ifdef __cplusplus
}
#endif

#endif /* end __BASE_LIB_SIGNAL_HELPER_H_ */

