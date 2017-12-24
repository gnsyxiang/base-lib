/****************************************************************
 * Filename: dir_helper.h
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 23/11 2017 23:07
 * Description:
 * 
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	23/11 2017
 ****************************************************************/
#ifndef _DIR_HELPER_H_
#define _DIR_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DIR_HELPER_GB
#define DIR_HELPER_EX extern
#else
#define DIR_HELPER_EX
#endif

typedef void (*handle)(const char* base_path, const char *name);


DIR_HELPER_EX void read_file_list(char *basePath, handle handle_file_dir);


#ifdef __cplusplus
}
#endif

#endif /* end _DIR_HELPER_H_ */

