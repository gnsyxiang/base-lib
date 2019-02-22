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
#include <assert.h>

#include "mem_helper.h"
#include "log_helper.h"

#define DIR_HELPER_GB
#include "dir_helper.h"
#undef DIR_HELPER_GB

#define CUR_DIR			"."
#define TOP_LEVLE_DIR	".."

int read_dir_file(const char * const path, 
        dir_file_cb_t dir_file_cb, void *args)
{
    assert(path);
    assert(dir_file_cb);

	DIR *dir;
	struct dirent *ptr;

	if ((dir = opendir(path)) == NULL) {
		log_e("open dir error");
        return -1;
	}

	while ((ptr = readdir(dir)) != NULL) {
		if (strcmp(ptr->d_name, CUR_DIR) == 0 \
                || strcmp(ptr->d_name, TOP_LEVLE_DIR) == 0) {
			continue;
        }

        switch (ptr->d_type) {
            case DT_REG:
                dir_file_cb(ptr->d_name, DT_REG, args);
                break;

            case DT_LNK:
                log_i("the soft link file");
                break;

            case DT_DIR: {
                int len = strlen(ptr->d_name) + strlen(path) + 1 + 1;	//1 for space('\0'), 1 for '/'
                char *sub_dir = alloc_mem(len);

                sprintf(sub_dir, "%s/%s", path, ptr->d_name);
                // log_i("path: %s, sub_dir: %s, ", path, sub_dir);

                dir_file_cb(ptr->d_name, DT_DIR, args);

                read_dir_file(sub_dir, dir_file_cb, args);

                free(sub_dir);
                break;
            }

            default:
                log_e("error for d_type");
                break;
        }
	}

	closedir(dir);

    return 0;
}

// int scandir(const char *dirp, struct dirent ***namelist,
        // int (*filter)(const struct dirent *),
        // int (*compar)(const struct dirent **, const struct dirent **));

int scan_dir_sort_file(const char * const dir, filter_t filter, 
		dir_file_cb_t dir_file_cb, void *args)
{
    assert(dir);
    assert(dir_file_cb);

	struct dirent **namelist; // struct dirent * namelist[];

	int num = scandir(dir, &namelist, filter, alphasort);
	if (num < 0) {
		log_e("scandir faild");
        return -1;
	} else {
		int cnt = 0;
		while (cnt < num) {
            // log_i("%s", namelist[cnt]->d_name);
            // log_i("%d", namelist[cnt]->d_type);
            switch (namelist[cnt]->d_type) {
                case DT_REG:
                    dir_file_cb(namelist[cnt]->d_name, namelist[cnt]->d_type, args);
                    break;

                case DT_DIR: {
                    int len = strlen(namelist[cnt]->d_name) + strlen(dir) + 1 + 1;	//1 for space('\0'), 1 for '/'
                    char *sub_dir = alloc_mem(len);

                    sprintf(sub_dir, "%s/%s", dir, namelist[cnt]->d_name);
                    // log_i("path: %s, sub_dir: %s, ", dir, sub_dir);

                    dir_file_cb(namelist[cnt]->d_name, namelist[cnt]->d_type, args);

                    scan_dir_sort_file(sub_dir, filter, dir_file_cb, args);

                    free(sub_dir);
                    break;
                }
                default:
                    break;
            }

			free(namelist[cnt++]);
		}
	}
	free(namelist);

    return 0;
}

int is_dir(const char * const filename)
{
    struct stat stat;

    lstat(filename, &stat);
    if (S_ISDIR(stat.st_mode)) {
        return 0;
    }

    // printf("%s size is : %ld\n", filename, stat.st_size);

    return -1;
}

