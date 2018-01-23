/**
 * 
 * Release under GPLv2.
 * 
 * @file    one-two-pointer.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/01 2018 16:35
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/01 2018      create the file
 * 
 *     last modified: 23/01 2018 16:35
 */
#ifndef _ONE_TWO_POINTER_H_
#define _ONE_TWO_POINTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ONE_TWO_POINTER_GB
#define ONE_TWO_POINTER_EX extern
#else
#define ONE_TWO_POINTER_EX
#endif

typedef struct teacher {
	char *name;         //名字
	char **course;       //所教授的课程，比如：数学，音乐，体育
	int age;
	int id;
}teacher;


ONE_TWO_POINTER_EX int teacher_init(teacher **p, int num);
ONE_TWO_POINTER_EX void teacher_destroy(teacher *p, int num);
#ifdef __cplusplus
}
#endif

#endif /* end _ONE_TWO_POINTER_H_ */

