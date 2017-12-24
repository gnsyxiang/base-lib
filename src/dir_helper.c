/****************************************************************
 * Filename: dir_helper.c
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 22/11 2017 23:40
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	22/11 2017
 ****************************************************************/
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

