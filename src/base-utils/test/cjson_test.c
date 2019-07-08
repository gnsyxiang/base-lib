/**
 * 
 * Release under GPLv2.
 * 
 * @file    cjson_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    11/07 2019 18:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        11/07 2019      create the file
 * 
 *     last modified: 11/07 2019 18:06
 */
#include <stdio.h>
#include <stdlib.h>

#include "cjson_wrapper.h"

const char * const json_err_str = "the given domain is error";
const int json_err_int = -1;
const double json_err_double = -1.0;

char text_json[]="{\n\
    \"asr_recongize\": \"加热模式\",\n\
    \"service\": \"cn.yunzhisheng.setting\",\n\
    \"code\": \"SETTING_EXEC\",\n\
    \"semantic\": {\n\
        \"intent\": {\n\
            \"operations\": [\n\
                {\n\
                    \"operator\": \"ACT_SET\",\n\
                    \"deviceType\": \"OBJ_AC\",\n\
                    \"value\": \"MODE_HEAT\",\n\
                    \"operands\": \"ATTR_MODE\"\n\
                }\n\
            ]\n\
        }\n\
    },\n\
    \"general\": {\n\
        \"type\": \"T\",\n\
        \"tts_ok\": [\n\
            \"已设为加热模式\"\n\
        ],\n\
        \"pcm_ok\": [\n\
            \"mode_heat.pcm\"\n\
        ]\n\
    },\n\
    \"test_int\":  800,\n\
    \"test_double\":  2.3\n\
}";

void cjson_test(void)
{
	cJSON *json = NULL;

	json=cJSON_Parse(text_json);

	char *out = cJSON_Print(json);
	printf("%s\n",out);
	free(out);

	cJSON *item_obj = cjson_get_item(json, "asr_recongize");
	if (item_obj)
		printf("image.thumbnail.url: %s\n", item_obj->valuestring);

	int height = cjson_get_item_int(json_err_int, json, "test_int");
	printf("Height: %d \n", height);

	double dret = cjson_get_item_double(json_err_double, json, "test_double");
	printf("dret: %f \n", dret);

	const char *test= cjson_get_item_char(json_err_str, json, "semantic", "intent", "operations", 0, "operator");
	printf("test: %s \n", test);

	cJSON_Delete(json);
}

