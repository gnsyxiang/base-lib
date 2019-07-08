/**
 * 
 * Release under GPLv2.
 * 
 * @file    signal_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    24/07 2019 13:49
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        24/07 2019      create the file
 * 
 *     last modified: 24/07 2019 13:49
 */
#ifndef __BASE_LIB_UTILS_INC_SIGNAL_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_SIGNAL_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_UTILS_SRC_SIGNAL_WRAPPER_GB
#define BASE_LIB_UTILS_SRC_SIGNAL_WRAPPER_EX extern
#else
#define BASE_LIB_UTILS_SRC_SIGNAL_WRAPPER_EX
#endif

BASE_LIB_UTILS_SRC_SIGNAL_WRAPPER_EX
void register_linux_signal_hanler(const char *app_name);

void free_linux_signal_hanler(void);

#ifdef __cplusplus
}
#endif

#endif /* end __BASE_LIB_UTILS_INC_SIGNAL_WRAPPER_H_ */

