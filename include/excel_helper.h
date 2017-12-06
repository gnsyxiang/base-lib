/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * All rights reserved.
 * 
 * @file    excel_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/12 2017 21:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/12 2017      create the file
 * 
 *     last modified: 06/12 2017 21:06
 */
#ifndef _EXCEL_HELPER_H_
#define _EXCEL_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXCEL_HELPER_GB
#define EXCEL_HELPER_EX extern
#else
#define EXCEL_HELPER_EX
#endif

typedef struct _excel_row_tag {
	FILE *fp;
	char *name;

	int num;
	int wakeup_flag;
	int asr_flag;
}excel_row_t;

excel_row_t *excel_open(char *name);
void excel_close(excel_row_t *row);

void excel_write_row(excel_row_t *row);
void excel_read_row(excel_row_t *row);

void excel_row_init(excel_row_t *row, 
		int num, char *name, 
		int wakeup_flag, int asr_flag);

#ifdef __cplusplus
}
#endif

#endif /* end _EXCEL_HELPER_H_ */

