/**
 *
 * Release under GPLv2.
 * 
 * @file    cjson_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/07 2019 17:28
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/07 2019      create the file
 * 
 *     last modified: 08/07 2019 17:28
 */
#ifndef __BASE_LIB_UTILS_INC_CJSON_WRAPPER_H_
#define __BASE_LIB_UTILS_INC_CJSON_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cJSON.h>
#include "macro_wrapper.h"

#ifndef BASE_LIB_UTILS_SRC_CJSON_WRAPPER_GB
#define BASE_LIB_UTILS_SRC_CJSON_WRAPPER_EX extern
#else
#define BASE_LIB_UTILS_SRC_CJSON_WRAPPER_EX
#endif

/**
 * 快速获得子json对象
 * @param json: 父json, 指向 cJSON 结构体的指针
 * @param x:    可变参数,用于描述子json在父json中的位置
 *              x: const char *: 表示一般的json对象name
 *              x: int: 表示json数组中的index
 * @return_val: 子json对象,如果不成功,那么返回NULL
 */
#define cjson_get_item(json, x...) cjson_get_item_va(json, macro_argc(x), x)

/**
 * cjson_get_item_char  : 快速获得子json对象的valuestring,必须是cJSON_String类型
 * cjson_get_item_int   : 快速获得子json对象的valueint,必须是cJSON_Number类型
 * cjson_get_item_double: 快速获得子json对象的valuedouble,必须是cJSON_Number类型
 * @param error_val: 解析错误时的返回值
 */
#define cjson_get_item_char(error_val, json, x...) \
        cjson_get_item_char_va(json, error_val, macro_argc(x), x)

#define cjson_get_item_int(error_val, json, x...) \
        cjson_get_item_int_va(json, error_val, macro_argc(x), x)

#define cjson_get_item_double(error_val, json, x...) \
        cjson_get_item_double_va(json, error_val, macro_argc(x), x)

/**
 * 
 * 例子:
 *   // json_str 的内容如下
 *   {
 *      "root" : {
 *          "child0" : {
 *              "child1" : {
 *                  "item0" : "hello obj",
 *                  "item1" : "hello obj again"
 *              }
 *          },
 *          "array" : {
 *              "item0" : "hello array",
 *              "item1" : "hello array again"
 *          }
 *      }
 *   }
 *
 *   cJSON *root = cJSON_Parse(json_str);
 *
 *   cJSON *item_obj = cJSON_GetItem(cjson, "root", "child0", "child1", "item0");
 *   if (item_obj)
 *      printf("root.child0.child1.item0: %s\n", item_obj->valuestring);
 *
 *   // "root.array[0].item0"
 *   cJSON *item_arr = cJSON_GetItem(cjson, "root", "array", 0, item0);
 *   if (item_arr)
 *      printf("root.array[0].item0: %s\n", item_arr->valuestring);
 *
 */

/*
 * functions called by helper macros, don't care those functions
 */
cJSON *cjson_get_item_va(cJSON *json, int n, ...);
const char *cjson_get_item_char_va(cJSON *json, const char *error_val, int n, ...);
int cjson_get_item_int_va(cJSON *json, int error_val, int n, ...);
double cjson_get_item_double_va(cJSON *json, double error_val, int n, ...);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_CJSON_WRAPPER_H_ */

