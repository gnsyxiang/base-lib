/**
 * 
 * Release under GPLv2.
 * 
 * @file    dir_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/07 2019 18:05
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/07 2019      create the file
 * 
 *     last modified: 02/07 2019 18:05
 */
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BASE_LIB_UTILS_SRC_DIR_WRAPPER_GB
#include "dir_wrapper.h"
#undef BASE_LIB_UTILS_SRC_DIR_WRAPPER_GB

#define DIR_LOG_TAG     "dir"

#define loge printf
#define logi printf
#define logw printf

static void _handle_file(struct dirent *ptr, handle_dir_t *handle_dir)
{
    switch (ptr->d_type) {
        case DT_REG:
            handle_dir->dir_file_cb(ptr->d_name, DT_REG, handle_dir->args);
            break;
        case DT_LNK:
            logi("the soft link file\n");
            break;
        case DT_DIR: {
            int len = strlen(ptr->d_name) + strlen(handle_dir->dir_path) + 1 + 1;	//1 for space('\0'), 1 for '/'

            handle_dir_t *handle_sub_dir = calloc(1, sizeof(handle_dir_t));
            handle_sub_dir->dir_file_cb  = handle_dir->dir_file_cb;
            handle_sub_dir->file_filter  = handle_dir->file_filter;
            handle_sub_dir->args         = handle_dir->args;
            handle_sub_dir->dir_path     = calloc(1, len);
            sprintf(handle_sub_dir->dir_path, "%s/%s", handle_dir->dir_path, ptr->d_name);

            handle_dir->dir_file_cb(ptr->d_name, DT_DIR, handle_sub_dir->args);
            read_dir(handle_sub_dir);

            free(handle_sub_dir->dir_path);
            free(handle_sub_dir);
            break;
        }
        default:
            logw("error for d_type \n");
            break;
    }
}

int read_dir(handle_dir_t *handle_dir)
{
    if (!handle_dir || !handle_dir->dir_path || !handle_dir->dir_file_cb) {
        loge("the param is NULL \n");
        return -1;
    }

    struct dirent *ptr;
    DIR *dir;

    if ((dir = opendir(handle_dir->dir_path)) == NULL) {
        loge("open dir error\n");
        return -1;
    }

    while ((ptr = readdir(dir)) != NULL) {
        if (strcmp(ptr->d_name, DOT) == 0 || strcmp(ptr->d_name, DOT2) == 0) {
            continue;
        }
        _handle_file(ptr, handle_dir);
    }
    closedir(dir);
    return 0;
}

int read_dir_filter_file(handle_dir_t *handle_dir)
{
    if (!handle_dir || !handle_dir->dir_path
            || !handle_dir->dir_file_cb || !handle_dir->file_filter) {
        loge("the param is NULL \n");
        return -1;
    }

    struct dirent **namelist; // struct dirent * namelist[];

    int num = scandir(handle_dir->dir_path, &namelist,
            handle_dir->file_filter, alphasort);
    if (num < 0) {
        loge("scandir faild \n");
        return -2;
    } else {
        int cnt = 0;
        while (cnt < num) {
            _handle_file(namelist[cnt], handle_dir);
            free(namelist[cnt++]);
        }
    }
    free(namelist);
    return 0;
}

int is_dir(const char * const path)
{
    struct stat stat;

    lstat(path, &stat);

    if (S_ISDIR(stat.st_mode)) {
        return 0;
    }

    return -1;
}

