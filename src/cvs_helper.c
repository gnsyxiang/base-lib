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

#define insert_valid_char(csv, ch)					\
	do {											\
		if (0 != str_insert_char(&csv->data, ch)) { \
			free(csv->data);						\
			return -1;								\
		}											\
	} while(0)

#define invalid_csv_format(csv)						\
	do {											\
		log_e("the CSV file is invalid one");		\
		free(csv->data);							\
		return -1;									\
	} while(0)

int parse_semicolon(csv_t *csv)
{
	int ch, c;

	while ((ch = fgetc(csv->fp)) != EOF) {
		if (STATUS_SEMICOLON == ch) {
			if ((c = fgetc(csv->fp)) != EOF) {
				invalid_csv_format(csv);
			}
			if (STATUS_SEMICOLON != n) {
				ungetc(n, csv->fp);
				break;
			}
			insert_valid_char(csv, ch);
		}

		if (STATUS_SEMICOLON != ch) {
			invalid_csv_format(csv);
		}
	}

	return 0;
}

int parse_csv(csv_t *csv)
{
	int ch;
	int rl, cl;

	while ((ch = fgetc(csv->fp)) != EOF) {
		switch (ch) {
			case STATUS_SEMICOLON:
				if (!parse_semicolon(csv))
					return -1;
				break;
			case STATUS_COMMA:
				insert_valid_char(csv, ch);
				++cl;
				break;
			case STATUS_WRAP_R:
				continue;
			case STATUS_WRAP_N:
				insert_valid_char(csv, ch);
				++cl;
				++rl;
				break;
			case STATUS_OK:
				insert_valid_char(csv, ch);
				break;
			default:
				break;
		}
	}

	csv->row = cl;
	csv->col = rl;

	return 0;
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

