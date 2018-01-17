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

typedef struct csv_matrix_tag {
	int row;
	int col;
} csv_matrix_t;

typedef struct csv_tag {
	FILE *fp;
	csv_matrix_t csv_matrix;
	const char *data[];
}csv_t;

#define CSV_MATRIX_TAG_LEN	(sizeof(csv_matrix_t))
#define CSV_TAG_LEN			(sizeof(csv_t))

csv_t *csv_file_create(const char *path, csv_matrix_t csv_matrix);
csv_t *csv_file_open(const char *path);
void csv_file_clean(csv_t *csv);

const char *csv_file_read_by_row_col(csv_t *csv, csv_matrix_t csv_matrix);
int csv_file_write_by_row_col(csv_t *csv, const char *buf, csv_matrix_t csv_matrix);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_CSV_HELPER_H_ */

