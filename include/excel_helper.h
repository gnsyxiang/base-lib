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
	int num;
	int wakeup_flag;
	int asr_flag;
	char *name;
}excel_row_t;

typedef struct _excel_tag {
	FILE *fp;
	excel_row_t *row;
	int rows;
	int columns;
}excel_t;

#define EXCEL_ROWS (4)
#define EXCEL_COLUMN (4)
#define FILE_NAME_LEN (256)
#define EXCEL_LEN (sizeof(excel_t))
#define EXCEL_ROW_LEN (sizeof(excel_row_t))

EXCEL_HELPER_EX excel_t *excel_open(char *name, int rows, int columns);
EXCEL_HELPER_EX void excel_close(excel_t *excel);
EXCEL_HELPER_EX void excel_seek(long offset, int whence);

EXCEL_HELPER_EX void excel_row_write(excel_row_t *row);
EXCEL_HELPER_EX void excel_row_read(excel_row_t *row);

EXCEL_HELPER_EX void excel_row_init(excel_row_t *row, 
									int num, char *name, 
									int wakeup_flag, int asr_flag);
EXCEL_HELPER_EX void excel_row_print(excel_row_t *row);
EXCEL_HELPER_EX void excel_print(excel_t *excel);

#ifdef __cplusplus
}
#endif

#endif /* end _EXCEL_HELPER_H_ */

