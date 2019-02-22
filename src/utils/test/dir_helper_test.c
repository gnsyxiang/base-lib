/**
 * 
 * Release under GPLv2.
 * 
 * @file    dir_helper_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    22/02 2019 17:09
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        22/02 2019      create the file
 * 
 *     last modified: 22/02 2019 17:09
 */
#include <stdio.h>
#include <string.h>

#include "parse_cmd.h"
#include "log_helper.h"
#include "dir_helper.h"

void dir_file_cb(const char * const name, unsigned char d_type, void *args)
{
    log_i("name: %s, args: %s", name, args);

    switch (d_type) {
        case DT_REG:
            break;

        case DT_DIR: 
            break;

        default:
            log_e("error for d_type");
            break;
    }
}

static void dir_test(void)
{
    read_dir_file(".", dir_file_cb, "1234");
}

static int filter_dot(const struct dirent *d)
{
    return (strcmp(d->d_name, "..") && strcmp(d->d_name, "."));
}

static void dir_filter_test(void)
{
    scan_dir_sort_file(".", filter_dot, dir_file_cb, "haha");
}

static void dir_test_init(void)
{
	printf(" 3. dir test \n");
	printf(" 4. dir filter test \n");

	handle_test_cmd_t dir_test_cmd[] = {
		{"3", dir_test},
		{"4", dir_filter_test},
	};

	register_test_cmd(dir_test_cmd, ARRAY_NUM(dir_test_cmd));
}
DECLARE_INIT(dir_test_init);

