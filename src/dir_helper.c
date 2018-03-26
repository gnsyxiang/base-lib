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
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "mem_helper.h"
#include "log_helper.h"

#define DIR_HELPER_GB
#include "dir_helper.h"
#undef DIR_HELPER_GB

#define CUR_DIR			"."
#define TOP_LEVLE_DIR	".."

/**
 * @brief traversing directory files, recursively calling
 *
 * @param base_path: specified directory
 * @param handle_file: callback func
 * @param handle_dir: callback func
 *
 * @return none
 */
void read_file_list(const char *base_path, handle_file_dir_t handle_cb)
{
	DIR *dir;
	struct dirent *ptr;

	if ((dir = opendir(base_path)) == NULL) {
		log_e("open dir error");
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL) {
		if (strcmp(ptr->d_name, CUR_DIR) == 0 || strcmp(ptr->d_name, TOP_LEVLE_DIR) == 0)
			continue;

		switch (ptr->d_type) {
			case DT_REG:
				if (handle_cb)
					handle_cb(base_path, ptr->d_name, ptr->d_type);
				break;

			case DT_DIR: {
				int len = strlen(ptr->d_name) + strlen(base_path) + 1 + 1;	//1 for space('\0'), 1 for '/'
				char *sub_dir = alloc_mem(len);

				sprintf(sub_dir, "%s/%s", base_path, ptr->d_name);
				/*log_i("base_path: %s, sub_dir: %s, ", base_path, sub_dir); */

				if (handle_cb)
					handle_cb(base_path, ptr->d_name, ptr->d_type);

				read_file_list(sub_dir, handle_cb);

				free(sub_dir);
				break;
			}

			default:
				break;
		}
	}

	closedir(dir);
}

/**
 * @brief scan the directory and process the file according to the filter rules
 *
 * @param dir_name: directory name
 * @param file_filter: the callback func of the filter rules
 * @param handle_file: callback func
 */
void scan_dir_sort_file(char *dir_name, file_filter_t file_filter, handle_file_dir_t handle_cb)
{
	struct dirent **namelist; // struct dirent * namelist[];

	int num = scandir(dir_name, &namelist, file_filter, alphasort);
	if (num < 0) {
		log_e("scandir faild");
	} else {
		int cnt = 0;
		while (cnt < num) {
			/*log_i("%s", namelist[cnt]->d_name);*/
			handle_cb(dir_name, namelist[cnt]->d_name, DT_REG);

			free(namelist[cnt++]);
		}
	}
	free(namelist);
}

