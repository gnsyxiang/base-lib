/**
 * 
 * Release under GPLv2.
 * 
 * @file    cvs_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/01 2018 23:22
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/01 2018      create the file
 * 
 *     last modified: 12/01 2018 23:22
 */
#include <stdio.h>

#include "csv_helper.h"
#include "parse_cmd.h"
#include "log_helper.h"
#include "csv_helper.h"

int csv_test(void)
{
	csv_matrix_t csv_matrix;
	csv_t *csv = csv_file_open("./test.csv");

	for (int i = 0; i < csv->csv_matrix.row; i++) {
		csv_matrix.row = i;
		for (int j = 0; j< csv->csv_matrix.col; j++) {
			csv_matrix.col = j;
			printf("<%d, %d> => [%s]\n", i, j, csv_file_read_by_row_col(csv, csv_matrix));
		}
	}
	return 0;
}

void csv_test_init(void)
{
	handle_test_cmd_t csv_test_cmd[] = {
		{"11", csv_test},
	};

	register_test_cmd(csv_test_cmd, ARRAY_NUM(csv_test_cmd));
}

