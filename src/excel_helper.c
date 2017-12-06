/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    excel_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/12 2017 20:05
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/12 2017      create the file
 * 
 *     last modified: 06/12 2017 20:05
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXCEL_HELPER_GB
#include "excel_helper.h"
#undef EXCEL_HELPER_GB

static inline void _excel_write_char(FILE *fp, char data)
{
	fprintf(fp, "%c\t", data);
}

static inline void _excel_write_int(FILE *fp, int data)
{
	fprintf(fp, "%d\t", data);
}

static inline void _excel_write_str(FILE *fp, char *data)
{
	fprintf(fp, "%s\t", data);
}

static inline void _excel_write_row(FILE *fp)
{
	fprintf(fp, "\n");
}

static inline void _excel_read_char(FILE *fp, char *data)
{
	fscanf(fp, "%c", data);
}

static inline void _excel_read_int(FILE *fp, int *data)
{
	fscanf(fp, "%d", data);
}

static inline void _excel_read_str(FILE *fp, char *data)
{
	fscanf(fp, "%s", data);
}

void excel_read_row(excel_row_t *row)
{
	_excel_read_int(row->fp, &row->num);
	_excel_read_str(row->fp, row->name);
	_excel_read_int(row->fp, &row->wakeup_flag);
	_excel_read_int(row->fp, &row->asr_flag);
}

void excel_write_row(excel_row_t *row)
{
	_excel_write_int(row->fp, row->num);
	_excel_write_str(row->fp, row->name);
	_excel_write_int(row->fp, row->wakeup_flag);
	_excel_write_int(row->fp, row->asr_flag);

	_excel_write_row(row->fp);
}

excel_row_t *excel_open(char *name)
{
	excel_row_t *row;

	row = (excel_row_t *)malloc(EXCEL_ROW_LEN);
	memset(row, '\0', EXCEL_ROW_LEN);

	row->name = (char *)malloc(FILE_NAME_LEN);
	memset(row->name, '\0', FILE_NAME_LEN);

	row->fp = fopen(name, "w+");

	return row;
}

void excel_close(excel_row_t *row)
{
	fclose(row->fp);

	free(row->name);
	free(row);
}

void excel_row_init(excel_row_t *row, 
		int num, char *name, 
		int wakeup_flag, int asr_flag)
{
	row->num = num;
	row->wakeup_flag = wakeup_flag;
	row->asr_flag = asr_flag;

	strcpy(row->name, name);
}

void excel_row_print(excel_row_t *row)
{
	printf("num: %d, name: %s, wakeup_flag: %d, asr_flag: %d \n", \
			row->num, row->name, row->wakeup_flag, row->asr_flag);
}

int main(int argc, char **argv)
{                   
	int i;
	excel_row_t *row;

	row = excel_open("test.xls");

	for (i = 0; i < EXCEL_ROWS; i++) {
		excel_row_init(row, i, "test.wav", 1, 1);
		excel_write_row(row);
	}

	fseek(row->fp, 0L, SEEK_SET);

	for(i = 0; i < EXCEL_ROWS; i++) {
		excel_read_row(row);
		excel_row_print(row);
	}

	excel_close(row);

	return 0;
}

