/**
 * 
 * Release under GPLv2.
 * 
 * @file    csv_helper.c
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
#include "csv_helper.h"
#include "str_helper.h"
#include "log_helper.h"

csv_t *csv_create_struct(void)
{
	csv_t *csv = alloc_mem(CSV_TAG_LEN);

	csv->csv_matrix.row = 0;
	csv->csv_matrix.col = 0;

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

#define insert_valid_char(str, ch)					\
	do {											\
		if (0 != str_insert_char(str, ch)) { \
			free_mem(str->buf);						\
			return NULL;								\
		}											\
	} while(0)

#define invalid_csv_format(str)						\
	do {											\
		log_e("the CSV file is invalid one");		\
		free_mem(str->buf);							\
		return NULL;									\
	} while(0)

int a;
void *parse_semicolon(FILE *fp, str_t *str)
{
	int ch, c;

	while ((ch = fgetc(fp)) != EOF) {
		if (STATUS_SEMICOLON == ch) {
			if ((c = fgetc(fp)) == EOF) {
				invalid_csv_format(str);
			}
			if (STATUS_SEMICOLON != c) {
				ungetc(c, fp);
				break;
			}
			insert_valid_char(str, ch);
		}

		if (STATUS_SEMICOLON != ch) {
			insert_valid_char(str, ch);
		}
	}

	//TODO handle it
	return (void *)&a;
}

str_t *parse_csv(csv_t *csv)
{
	int ch;
	int row = 0, col = 0;
	FILE *fp = csv->fp;
	str_t *str = str_create_by_len(0);

	while ((ch = fgetc(fp)) != EOF) {
		switch (ch) {
			case STATUS_SEMICOLON:
				if (!parse_semicolon(fp, str))
					return NULL;
				break;
			case STATUS_COMMA:
				insert_valid_char(str, '\0');
				++col;
				break;
			case STATUS_WRAP_R:
				continue;
			case STATUS_WRAP_N:
				insert_valid_char(str, '\0');
				++col;
				++row;
				break;
			default:
				insert_valid_char(str, ch);
				break;
		}
	}

	csv->csv_matrix.col = col;
	csv->csv_matrix.row = row;

	return str;
}

void parse_csv_data(csv_t *csv, str_t *str)
{
	int i = 0;
	char *buf = str->buf;
	int col = csv->csv_matrix.col;
	int row = csv->csv_matrix.row;

	csv->csv_matrix.col = col / row;

	do {
		csv->data[i] = buf;
		while (*buf++);
	} while(++i < col);
}


csv_t *csv_file_open(const char *path)
{
	csv_t *csv = csv_create_struct();

	csv->fp = fopen_l(path, "r");
	str_t *str = parse_csv(csv);
	parse_csv_data(csv, str);

	return csv;
}

void csv_clean(csv_t *csv)
{
}

const char *csv_file_read_by_row_col(csv_t *csv, csv_matrix_t csv_matrix)
{
	int row = csv_matrix.row;
	int col = csv_matrix.col;

	return csv->data[row * csv->csv_matrix.col + col];
}

