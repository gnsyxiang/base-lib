/**
 * 
 * Release under GPLv2.
 * 
 * @file    cjson_utils.c
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
#include <stdio.h>
#include <stdlib.h>

#include "log_helper.h"
#include "file_helper.h"
#include "mem_helper.h"

#include "cjson_utils.h"

cJSON *cjson_get_object_from_file(const char *path)
{
	cJSON *json = NULL;

	if (!path) {
		log_i("the file is NULL \n");
		return NULL;
	}

	char* content;
	long len = open_file_get_len(path);
	if (len > 0) {
		content = alloc_mem(len);

		FILE *fp = fopen(path, "r");
		fread(content, 1, len, fp);
		fclose(fp);

		json = cJSON_Parse(content);
		if (!json) {
			log_i("cJSON_Parse faild");
			return NULL;
		}

		free_mem(content);
	}

	return json;
}

