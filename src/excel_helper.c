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

void write_excel(FILE *fp)
{
	int chy[4]={2, 4, 6, 8};
	int data[4]={1, 3, 5, 7};
	int i;

	for (i = 0; i < 4; i++)
		fprintf(fp, "%d\t%d\n", chy[i], data[i]);
}

void read_excel(FILE *fp)
{   
	int i, j;
	int da[4][2] = {0};

	fseek(fp, 0L, SEEK_SET);

	for(i = 0; i < 4; i++)
		for(j = 0; j < 2; j++)
			fscanf(fp,"%d",&da[i][j]);

	for(i = 0; i < 4; i++)
		printf("%d\t%d\n", da[i][0], da[i][1]);
}

void main()
{                   
	FILE *fp = fopen("test.xls","w+") ;

	write_excel(fp); 
	read_excel(fp);

	fclose(fp);
}
