/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    dir_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:35
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:35
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIR_HELPER_GB
#include "dir_helper.h"
#undef DIR_HELPER_GB

void read_file_list(char *basePath, handle handle_file_dir)
{
    DIR *dir;
    struct dirent *ptr;

    if ((dir = opendir(basePath)) == NULL) {
        perror("open dir error...");
        exit(1);
    }

    while ((ptr = readdir(dir)) != NULL) {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
            continue;
        else if(ptr->d_type == DT_REG) {
            handle_file_dir(basePath, ptr->d_name);
        } else if(ptr->d_type == DT_DIR) {
            int len = strlen(ptr->d_name) + strlen(basePath) + 20;
            char *sub_dir = (char *)malloc(len);
            sprintf(sub_dir, "%s/%s", basePath, ptr->d_name);

            /* printf("base_path: %s, sub_dir: %s, len: %d \n", basePath, sub_dir, len); */
            read_file_list(sub_dir, handle_file_dir);

            free(sub_dir);
        }
    }

    closedir(dir);
}

