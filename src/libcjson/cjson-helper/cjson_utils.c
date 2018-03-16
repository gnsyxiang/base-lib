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

#include "cjson_utils.h"

cJSON *GetJsonObject(const char *cjson_name, cJSON *json)
{
	long len;
	char* pContent;
	int tmp;
	FILE *fp;

	if (!cjson_name) {
		printf("%s is NULL \n", cjson_name);
		return NULL;
	}

	if (!(fp = fopen(cjson_name, "rb+"))) {
		printf("fopen %s faild \n", cjson_name);
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	if(0 == len)
	{
		return NULL;
	}

	fseek(fp,0,SEEK_SET);
	pContent = (char*) malloc (sizeof(char) * len);
	tmp = fread(pContent,1,len,fp);
	printf("tmp: %d \n", tmp);

	fclose(fp);
	json=cJSON_Parse(pContent);
	if (!json)
	{
		return NULL;
	}
	free(pContent);
	return json;
}

