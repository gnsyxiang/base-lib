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

/*#define XIA_DEBUG*/

static FILE *fp;
static str_t *str;

csv_t *csv_create_struct(int len)
{
	csv_t *csv = alloc_mem(CSV_TAG_LEN + len);

	csv->row = 0;
	csv->col = 0;

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

str_t *parse_csv(int *prow, int *pcol)
{
	int ch;
	int row = 0, col = 0;
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

	*prow = row;
	*pcol = col;

	return str;
}

csv_t *parse_csv_data(str_t *str, int row, int col)
{
#ifdef XIA_DEBUG
	log_i("row: %d", row);
	log_i("col: %d", col);
#endif

	int i = 0;
	char *buf = str->buf;
	csv_t *csv = csv_create_struct(col * DATA_POINT_LEN);

	csv->row = row;
	csv->col = col / row;

	do {
		csv->data[i] = buf;
		while (*buf++);
	} while(++i < col);

	return csv;
}

csv_t *csv_file_open(const char *path)
{
	int row = 0, col = 0;

	fp = fopen_l(path, "r");
	str = parse_csv(&row, &col);

	return parse_csv_data(str, row, col);
}

void csv_file_clean(csv_t *csv)
{
	fclose_l(fp);

	free_mem(str->buf);
	free_mem(str);

	free_mem(csv);
}

const char *csv_file_read_by_row_col(csv_t *csv, int row, int col)
{
	return csv->data[row * csv->col + col];
}

