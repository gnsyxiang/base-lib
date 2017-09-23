/****************************************************************
 * Filename: misc_helper.h
 * -------------------------------
 * Copyright(C),
 * Author: zhenquan.qiu
 * Version: V1.0.0
 * Last modified: 22/09 2017 21:54
 * Description:
 *
 * Change Log:
 * NO.	Author		    Date		Modified
 * 00	zhenquan.qiu	22/09 2017
 ****************************************************************/

#ifndef _MISC_HELPER_H_
#define _MISC_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MISC_HELPER_GB
#define MISC_HELPER_EX extern
#else
#define MISC_HELPER_EX
#endif

#define FOR_ZERO_TO_LEN(len) for (int i = 0; i < (len); i++)

MISC_HELPER_EX void test(void);

#ifdef __cplusplus
}
#endif

#endif /* end _MISC_HELPER_H_ */

