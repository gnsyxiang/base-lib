/**
 * 
 * Release under GPLv2.
 * 
 * @file    one-two-pointer.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/01 2018 16:34
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/01 2018      create the file
 * 
 *     last modified: 23/01 2018 16:34
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define ONE_TWO_POINTER_GB
#include "one-two-pointer.h"
#undef ONE_TWO_POINTER_GB

#define NAME_LEN (60)
#define COURSE_NUM (3)
#define COURSE_LEN (30)

int main(int argc, char **argv)
{
    teacher *p;

    teacher_init(&p, 3);

    teacher_destroy(p, 3);

    return 0;
}

int teacher_init(teacher **p, int num)
{
    assert(NULL != p);

    teacher *tmp = NULL;
    tmp = (teacher *)malloc(sizeof(teacher) * num);
    if (NULL == tmp) {
        printf("malloc faild \n");
        return -1;
    }

    memset(tmp, 0, (sizeof(teacher) * num));

    for (int i = 0; i < num; i++) {
        /* 分配一级指针 */
        tmp[i].name = (char *)malloc(NAME_LEN);
        if (NULL == tmp[i].name) {
            printf("malloc faild \n");
            return -1;
        }

        /* 打造二级指针 */
        char **pt = NULL;
        pt = (char **)malloc(COURSE_NUM * sizeof(char *));
        if (NULL == pt) {
            printf("malloc faild \n");
            return -1;
        }

        for (int j = 0; j < COURSE_NUM; j++) {
            pt[j] = (char *)malloc(COURSE_LEN);
        }

        /* 挂上去 */
        tmp[i].course = pt;
    }

    /* p是二级指针，*p就是一级指针 */
    *p = tmp;

    return 0;
}

void teacher_destroy(teacher *p, int num)
{
    if (!p)
        return;

    for (int i = 0; i < num; i++) {
        if (p[i].name) {
            free(p[i].name);
        }

        if (p[i].course) {
            char **tmp = p[i].course;
            for (int j = 0; j < COURSE_NUM; j++) {
                if (tmp[j]) {
                    free(tmp[j]);
                }
            }
            free(tmp);
            p[i].course = NULL;
        }
    }

    free(p);
}


