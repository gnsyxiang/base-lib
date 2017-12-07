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

static FILE *fp;

static inline void _excel_write_char(char data)
{
	fprintf(fp, "%c\t", data);
}

static inline void _excel_write_int(int data)
{
	fprintf(fp, "%d\t", data);
}

static inline void _excel_write_str(char *data)
{
	fprintf(fp, "%s\t", data);
}

static inline void _excel_write_row(void)
{
	fprintf(fp, "\n");
}

static inline void _excel_read_char(char *data)
{
	fscanf(fp, "%c", data);
}

static inline void _excel_read_int(int *data)
{
	fscanf(fp, "%d", data);
}

static inline void _excel_read_str(char *data)
{
	fscanf(fp, "%s", data);
}

void excel_row_read(excel_row_t *row)
{
	_excel_read_int(&row->num);
	_excel_read_str(row->name);
	_excel_read_int(&row->wakeup_flag);
	_excel_read_int(&row->asr_flag);
}

void excel_row_write(excel_row_t *row)
{
	_excel_write_int(row->num);
	_excel_write_str(row->name);
	_excel_write_int(row->wakeup_flag);
	_excel_write_int(row->asr_flag);

	_excel_write_row();
}

excel_t *excel_open(char *name)
{
	excel_t *excel;

	excel = (excel_t *)malloc(EXCEL_LEN);
	memset(excel, '\0', EXCEL_LEN);

	excel->row = (excel_row_t *)malloc(EXCEL_ROW_LEN);
	memset(excel->row, '\0', EXCEL_ROW_LEN);

	excel->row->name = (char *)malloc(FILE_NAME_LEN);
	memset(excel->row->name, '\0', FILE_NAME_LEN);

	excel->fp = fopen(name, "w+");
	fp = excel->fp;

	return excel;
}

void excel_close(excel_t *excel)
{
	fclose(excel->fp);

	free(excel->row->name);
	free(excel->row);
	free(excel);
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

void excel_seek(long offset, int whence)
{
	fseek(fp, offset, whence);
}

int main(int argc, char **argv)
{                   
	int i;
	excel_t *excel;

	excel = excel_open("test.xls");

	for (i = 0; i < EXCEL_ROWS; i++) {
		excel_row_init(excel->row, i, "test.wav", 1, 1);
		excel_row_write(excel->row);
	}

	excel_seek(0L, SEEK_SET);

	for(i = 0; i < EXCEL_ROWS; i++) {
		excel_row_read(excel->row);
		excel_row_print(excel->row);
	}

	excel_close(excel);

	return 0;
}

