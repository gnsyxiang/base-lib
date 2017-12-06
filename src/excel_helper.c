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

int main(int argc, char **argv)
{                   
	int i;
	excel_row_t *row;

	row = (excel_row_t *)malloc(sizeof(excel_row_t));
	memset(row, '\0', sizeof(excel_row_t));

	row->name = (char *)malloc(20);
	memset(row->name, '\0', 20);

	row->fp = fopen("test.xls","w+") ;

	row->num = 1;
	row->wakeup_flag = 1;
	row->asr_flag = 1;

	strcpy(row->name, "test.wav");

	for (i = 0; i < 4; i++)
		excel_write_row(row);

	fseek(row->fp, 0L, SEEK_SET);

	for(i = 0; i < 4; i++) {
		excel_read_row(row);
		printf("num: %d, name: %s, wakeup_flag: %d, asr_flag: %d \n", \
				row->num, row->name, row->wakeup_flag, row->asr_flag);
	}

	fclose(row->fp);

	free(row->name);
	free(row);

	return 0;
}

