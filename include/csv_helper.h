/**
 *
 * Release under GPLv2.
 * 
 * @file    csv_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    11/01 2018 22:17
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        11/01 2018      create the file
 * 
 *     last modified: 11/01 2018 22:17
 */
#ifndef __BASE_LIB_CSV_HELPER_H_
#define __BASE_LIB_CSV_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "str_buf_helper.h"

typedef struct csv_tag {
	FILE *fp;
	int row;
	int col;
	str_buf_t *str;
	const char *data[];
}csv_t;

#define CSV_TAG_LEN			(sizeof(csv_t))
#define DATA_POINT_LEN		(sizeof(const char *))

csv_t *csv_file_create(const char *path, int row, int col);
csv_t *csv_file_open(const char *path);
void csv_file_clean(csv_t *csv);

const char *csv_file_read_by_row_col(csv_t *csv, int row, int col);
void csv_file_read_row(csv_t *csv, int row);
int csv_file_write_by_row_col(csv_t *csv, const char *buf, int row, int col);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_CSV_HELPER_H_ */

