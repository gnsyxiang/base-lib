/**
 *
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

#include "mem_helper.h"
#include "log_helper.h"

#define DIR_HELPER_GB
#include "dir_helper.h"
#undef DIR_HELPER_GB

/**
 * @brief traversing directory files, recursively calling
 *
 * @param basePath: specified directory
 * @param handle_file_dir: callback func
 * @param wav_handle_cb
 *
 * @return none
 */
void read_file_list(const char *basePath, wav_handle_t handle_file_dir, wav_handle_cb_t wav_handle_cb)
{
	DIR *dir;
	struct dirent *ptr;

	if ((dir = opendir(basePath)) == NULL) {
		log_e("open dir error");
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL) {
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
			continue;
		else if(ptr->d_type == DT_REG) {
			handle_file_dir(basePath, ptr->d_name, wav_handle_cb);
		} else if(ptr->d_type == DT_DIR) {
			int len = strlen(ptr->d_name) + strlen(basePath) + 20;
			char *sub_dir = alloc_mem(len);

			sprintf(sub_dir, "%s/%s", basePath, ptr->d_name);

			/*printf("base_path: %s, sub_dir: %s, len: %d \n", basePath, sub_dir, len); */
			read_file_list(sub_dir, handle_file_dir, wav_handle_cb);

			free(sub_dir);
		}
	}

	closedir(dir);
}

