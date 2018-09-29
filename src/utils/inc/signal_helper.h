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
#ifndef __UTILS_SIGNAL_HELPER_H_
#define __UTILS_SIGNAL_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UTILS_SIGNAL_GB
#define UTILS_SIGNAL_EX extern
#else
#define UTILS_SIGNAL_EX
#endif

UTILS_SIGNAL_EX void register_linux_signal_hanler(const char *app_name);

#ifdef __cplusplus
}
#endif

#endif /* end __UTILS_SIGNAL_HELPER_H_ */

