/**
 * 
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/07 2019 14:34
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/07 2019      create the file
 * 
 *     last modified: 03/07 2019 14:34
 */
#include <stdio.h>
#include <string.h>

#include "dir_wrapper.h"

// 过滤掉"."和".."
static int filter_dot(const struct dirent *d)
{
    return (strcmp(d->d_name, DOT) && strcmp(d->d_name, DOT2));
}

// 文件筛选器，筛选“.o”文件
static int filter_o(const struct dirent *d)
{
    char name[] = ".o";

    if (0 == filter_dot(d)) {
        return 0;
    }

    size_t len = strlen(d->d_name);
    if (len >= 2 && d->d_name[len - 2] == name[0] && d->d_name[len - 1] == name[1]) {
        return -1;
    }
    return 0;
}

void dir_file_cb(char *name, unsigned char d_type, void *args)
{
    printf("name: %s, d_type: %d \n", name, d_type);
}

int dir_test(void)
{
    int ret = is_dir("./");
    printf("is_dir: %d \n", ret);

    handle_dir_t handle_dir;
    handle_dir.dir_path     = "./";
    handle_dir.dir_file_cb  = dir_file_cb;
    handle_dir.args         = NULL;
    handle_dir.file_filter  = filter_o;

    read_dir(&handle_dir);
    read_dir_filter_file(&handle_dir);

    return 0;
}

