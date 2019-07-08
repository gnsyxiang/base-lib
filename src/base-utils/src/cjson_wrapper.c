/**
 * 
 * Release under GPLv2.
 * 
 * @file    cjson_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/07 2019 17:27
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/07 2019      create the file
 * 
 *     last modified: 08/07 2019 17:27
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define BASE_LIB_UTILS_SRC_CJSON_WRAPPER_GB
#include "cjson_wrapper.h"
#undef BASE_LIB_UTILS_SRC_CJSON_WRAPPER_GB

static cJSON *_cjson_get_item_valist(cJSON *json, int n, va_list list)
{
    cJSON *js = json;

    for (int i = 0; i < n; i++) {
        if (js->type == cJSON_Array) {
            int index = va_arg(list, int);
            js = cJSON_GetArrayItem(js, index);
        } else {
            const char *str = va_arg(list, const char *);
            js = cJSON_GetObjectItem(js, str);
        }

        if (js == NULL)
            break;
    }

    return js;
}

#define _cjson_get_item(json, n) ({                    \
    va_list list;                                      \
    va_start(list, n);                                 \
    cJSON *js = _cjson_get_item_valist(json, n, list); \
    va_end(list);                                      \
    js;                                                \
    })

cJSON *cjson_get_item_va(cJSON *json, int n, ...)
{
    return _cjson_get_item(json, n);
}

const char *cjson_get_item_char_va(cJSON *json, const char *error_val, int n, ...)
{
    cJSON *js = _cjson_get_item(json, n);

    if (!js || js->type != cJSON_String)
        return error_val;
    else
        return js->valuestring;
}

int cjson_get_item_int_va(cJSON *json, int error_val, int n, ...)
{
    cJSON *js = _cjson_get_item(json, n);

    if (!js || js->type != cJSON_Number)
        return error_val;
    else
        return js->valueint;
}

double cjson_get_item_double_va(cJSON *json, double error_val, int n, ...)
{
    cJSON *js = _cjson_get_item(json, n);

    if (!js || js->type != cJSON_Number)
        return error_val;
    else
        return js->valuedouble;
}

