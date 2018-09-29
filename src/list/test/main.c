/**
 * 
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/09 2018 09:03
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/09 2018      create the file
 * 
 *     last modified: 29/09 2018 09:03
 */
#include <stdio.h>
#include <string.h>

#include <log_helper.h>

#include "list.h" 

#define MAX_NAME_LEN 32
#define MAX_ID_LEN 10

typedef struct stud {
    struct list_head list;

    char name[MAX_NAME_LEN];
    char id[MAX_ID_LEN];
} stud_t;

int main(int argc, char const* argv[])
{
    stud_t stu_1;
    stud_t stu_2;

    struct list_head head;
    INIT_LIST_HEAD(&head);

    strcpy(stu_1.name, "lisi");
    strcpy(stu_1.id, "01");

    strcpy(stu_2.name, "zhangsan");
    strcpy(stu_2.id, "02");

    list_add(&stu_1.list, &head);
    list_add(&stu_2.list, &head);

    list_del(&stu_2.list);

    stud_t *entry;
    struct list_head *p;
    list_for_each(p,&head) {
        entry = list_entry(p, struct stud, list);

        log_i("name: %s", entry->name);
        log_i("stu_number: %s", entry->id);
    }

    list_del(&stu_1.list);

    return 0;
}

