/**
 * 
 * Release under GPLv2.
 * 
 * @file    cvs_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/01 2018 16:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/01 2018      create the file
 * 
 *     last modified: 08/01 2018 16:48
 */

/*
 * csv文件
 * ------------------------------------------
 * |	a	|	b	|	c	|	d			|
 * ------------------------------------------
 * | 编号	| 姓名	|  年龄 | 备注			|
 * ------------------------------------------
 * | 1		| 王志	| 18	| 你好"",,好的'"|
 * ------------------------------------------
 * | 2		|"你
 * |		  好"   | 19	|就这样			|
 * ------------------------------------------
 * | 3		| 测试  | 520   |算了吧			|
 * ------------------------------------------
 * | 4		| 就这  |		|"可以			|
 * ------------------------------------------
 *
 * txt文件
 * -----------------------------
 * 编号,姓名,年龄,备注
 * 1,王志,18,"你好"""",,好的'"""
 * 2,"你
 * 好",19,就这样
 * 3,测试,520,算了吧
 * 4,就这,,"""可以"
 * -----------------------------
 *
 * cvs文件的编码规则：
 * 1，用","分割每列
 * 2, 如果出现",|"|\n"这种特殊字符，该单元就会用""包裹起来，并且里面的引号，两个表示一个（转义）
 * 3, 每行用"\r\n"结束
 */

#include <stdio.h>

#include "mem_helper.h"
#include "file_helper.h"

typedef struct csv_tag {
	FILE *fp;
	int row;
	int col;
	const char *data[];
}csv_t;

#define CSV_TAG_LEN (sizeof(csv_t))

csv_t *csv_create_struct(void)
{
	csv_t *csv = alloc_mem(CSV_TAG_LEN);

	csv->row = 0;
	csv->col = 0;

	csv->fp = NULL;
	//csv->data = NULL;

	return csv;
}

enum {
	STATUS_SEMICOLON = '"',
	STATUS_COMMA = ',',
	STATUS_WRAP_R = '\r',
	STATUS_WRAP_N = '\n',

	STATUS_OK,
};

void parse_csv(csv_t *csv)
{
	int ch, c;
	int rl, cl;

	while ((ch = fgetc(csv->fp)) != EOF) {
		switch (ch) {
			case STATUS_SEMICOLON:
				break;
			case STATUS_COMMA:
				break;
			case STATUS_WRAP_R:
				break;
			case STATUS_WRAP_N:
				break;
			case STATUS_OK:
				break;
			default:
				break;
		}
	}
}

csv_t *csv_new(const char *path)
{
	csv_t *csv = csv_create_struct();

	csv->fp = fopen_l(path, "r");

	return csv;
}

void csv_clean(csv_t *csv)
{
}

const char *csv_get_data(csv_t *csv, int row, int col)
{
	return NULL;
}


















