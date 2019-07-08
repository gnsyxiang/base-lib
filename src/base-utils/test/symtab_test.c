/**
 * 
 * Release under GPLv2.
 * 
 * @file    symtab_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/07 2019 14:03
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/07 2019      create the file
 * 
 *     last modified: 23/07 2019 14:03
 */
#include <stdio.h>
#include <stdlib.h>

#include "symtab.h"

char *key[] = { "1", "2", "3", "4", "5"};

#define NBuckets    (10)

void symtab_user_cb(char *key, void *value, void *args)
{
    printf("key: %s, value: %s \n", key, (char *)value);
}

void symtab_test(void)
{
    char buf[5];
    void *handle;
    symtab_init(&handle, NBuckets);
    for (int i = 0; i < 5; i++) {
        sprintf(buf, "%d", i);
        symtab_insert(handle, key[i], &buf);
    }
    symtab_map_user_display(handle, symtab_user_cb, NULL);
    for (int i = 0; i < 5; i++) {
        char *buf = symtab_remove(handle, key[i]);
        printf("buf: %s \n", buf);
        free(buf);
    }
    symtab_final(handle);
}

