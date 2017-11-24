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
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef void (*handle)(const char* base_path, const char *name);

void read_file_list(char *basePath, handle handle_file_dir)
{
    DIR *dir;
    struct dirent *ptr;

    if ((dir = opendir(basePath)) == NULL) {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr = readdir(dir)) != NULL) {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
            continue;
        else if(ptr->d_type == DT_REG) {
            printf("base_path: %s, name: %s \n", basePath, ptr->d_name);

            handle_file_dir(basePath, ptr->d_name);
        }
        /* else if(ptr->d_type == 4) { */
            /* memset(base,'\0',sizeof(base)); */
            /* strcpy(base,basePath); */
            /* strcat(base,"/"); */
            /* strcat(base,ptr->d_name); */
            /* read_file_list(base, handle_file_dir); */
        /* } */
    }

    closedir(dir);
}

extern void wav_handle(const char *base_path, const char *name);

int main(void)
{
    char basePath[1000];

    memset(basePath,'\0',sizeof(basePath));
    getcwd(basePath, 999);
    printf("the current dir is : %s\n",basePath);

    ///get the file list
    memset(basePath,'\0',sizeof(basePath));
    strcpy(basePath,"./wav");

    read_file_list(basePath, wav_handle);

    return 0;
}
