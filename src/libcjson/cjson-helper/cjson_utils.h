/**
 *
 * Release under GPLv2.
 * 
 * @file    cjson_utils.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/03 2018 15:53
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/03 2018      create the file
 * 
 *     last modified: 14/03 2018 15:53
 */
#ifndef __BASE_LIB_CJSON_H_
#define __BASE_LIB_CJSON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cJSON.h"

cJSON *GetJsonObject(const char *cjson_name, cJSON *json);

#ifdef __cplusplus
}
#endif

#endif /* end __BASE_LIB_CJSON_H_ */

