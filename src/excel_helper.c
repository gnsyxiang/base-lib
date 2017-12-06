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

void excel_write_row(FILE *fp, int num, char *name, int wakeup_flag, int asr_flag)
{
	_excel_write_int(fp, num);
	_excel_write_str(fp, name);
	_excel_write_int(fp, wakeup_flag);
	_excel_write_int(fp, asr_flag);

	_excel_write_row(fp);
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

void excel_read_row(FILE *fp, int *num, char *name, int *wakeup_flag, int *asr_flag)
{
	_excel_read_int(fp, num);
	_excel_read_str(fp, name);
	_excel_read_int(fp, wakeup_flag);
	_excel_read_int(fp, asr_flag);
}

void write_excel(FILE *fp)
{
	int i;

	for (i = 0; i < 4; i++)
		excel_write_row(fp, 1, "test.wav", 1, 1);
}

void read_excel(FILE *fp)
{   
	int i;
	int num;
	char name[30] = {0};
	int wakeup_flag;
	int asr_flag;

	fseek(fp, 0L, SEEK_SET);

	for(i = 0; i < 4; i++) {
		excel_read_row(fp, &num, name, &wakeup_flag, &asr_flag);
		printf("num: %d, name: %s, wakeup_flag: %d, asr_flag: %d \n", num, name, wakeup_flag, asr_flag);
	}
}

void main()
{                   
	FILE *fp = fopen("test.xls","w+") ;

	write_excel(fp); 
	read_excel(fp);

	fclose(fp);
}
