/**
 * 
 * Release under GPLv2.
 * 
 * @file    symtab.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    22/07 2019 15:51
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        22/07 2019      create the file
 * 
 *     last modified: 22/07 2019 15:51
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log_wrapper.h"
#include "list.h"

#define BASE_LIB_UTILS_SRC_SYMTAB_GB
#include "symtab.h"
#undef BASE_LIB_UTILS_SRC_SYMTAB_GB

#define LOG_SYMTAB_TAB  "symtab"
#define KEY_LEN         (256)
#define VAL_LEN         (256)

char undefined_object[] = "UNDEFINED";
#define UNDEFINED ((void *) undefined_object)

#define NBuckets    (10)

typedef struct cell {
    char *key;
    void *value;

    struct hlist_node hlist;
} cell_t;

typedef struct symtab {
    int num;

    struct hlist_head hlist_head[NBuckets];
}symtab_t;

int symtab_init(void **handle, int num)
{
    symtab_t *symtab;

    symtab = calloc(1, sizeof(symtab_t));
    symtab->num = num;

    logi(LOG_SYMTAB_TAB, "symtab_init success, the num: %d \n", symtab->num);

    *handle = (void *)symtab;

    return 0;
}

static inline void _cell_final(cell_t *cell)
{
    free(cell->key);
    free(cell->value);
    free(cell);
}

static void _free_bucket_chain(struct hlist_head *hlist_head)
{
    cell_t *pos;
    struct hlist_node *n;

    hlist_for_each_entry_safe(pos, n, hlist_head, hlist) {
        hlist_del(&pos->hlist);
        _cell_final(pos);
    }
}

void symtab_final(void *handle)
{
    symtab_t *symtab = (symtab_t *)handle;

    if (!handle) {
        loge(LOG_SYMTAB_TAB, "the param is error \n");
        return;
    }
    int i;
    for (i = 0; i < NBuckets; i++) {
        _free_bucket_chain(&symtab->hlist_head[i]);
    }

    free(symtab);
}

static cell_t *_cell_init(char *key)
{
    cell_t *cell;

    cell = calloc(1, sizeof(cell_t));
    cell->key = calloc(1, KEY_LEN);
    cell->value = calloc(1, VAL_LEN);
    strncpy(cell->key, key, strlen(key));
    INIT_HLIST_NODE(&cell->hlist);

    return (cell);
}

static cell_t * _find_cell(struct hlist_head *hlist_head, char *key)
{
    cell_t *pos = NULL;
    hlist_for_each_entry(pos, hlist_head, hlist) {
        if (pos && !strncmp(pos->key, key, strlen(pos->key))) {
            break;
        }
    }
    return (pos);
}

#define Multiplier (-1664117991L)
static int hash(char *str, int num)
{
    int i;
    unsigned long hashcode = 0;

    for (i = 0; str[i] != '\0'; i++) {
        hashcode = hashcode * Multiplier + str[i];
    }

    return (hashcode % num);
}

void symtab_insert(void *handle, char *key, void *value)
{
    int num;
    cell_t *cell;
    symtab_t *symtab = (symtab_t *)handle;

    if (!handle) {
        loge(LOG_SYMTAB_TAB, "the param is error \n");
        return;
    }

    num = hash(key, symtab->num);
    cell = _find_cell(&symtab->hlist_head[num], key);
    if (!cell) {
        cell = _cell_init(key);
    }
    strncpy(cell->value, value, strlen(value));

    hlist_add_head(&cell->hlist, &symtab->hlist_head[num]);
}

static char *_symtab_remove_common(void *handle, char *key, int peek_flag)
{
    cell_t *pos = NULL;
    struct hlist_node *n;
    int num = -1;
    char *val = NULL;

    symtab_t *symtab = (symtab_t *)handle;

    num = hash(key, symtab->num);
    hlist_for_each_entry_safe(pos, n, &symtab->hlist_head[num], hlist) {
        if (pos && !strncmp(pos->key, key, strlen(pos->key))) {
            val = calloc(1, strlen(pos->value) + 1);
            strcpy(val, pos->value);
            if (!peek_flag) {
                hlist_del(&pos->hlist);
                _cell_final(pos);
            }
            break;
        }
    }

    return val;
}

void *symtab_remove_peek(void *handle, char *key)
{
    if (!handle) {
        loge(LOG_SYMTAB_TAB, "the param is error \n");
        return NULL;
    }

    return _symtab_remove_common(handle, key, 1);
}

void *symtab_remove(void *handle, char *key)
{
    if (!handle) {
        loge(LOG_SYMTAB_TAB, "the param is error \n");
        return NULL;
    }

    return _symtab_remove_common(handle, key, 0);
}

void symtab_map_user_display(void *handle, symtab_user_cb_t symtab_user_cb, void *args)
{
    symtab_t *symtab = (symtab_t *)handle;

    if (!handle || !symtab_user_cb) {
        loge(LOG_SYMTAB_TAB, "the param is error \n");
    }

    int i;
    cell_t *pos;
    for (i = 0; i < symtab->num; i++) {
        hlist_for_each_entry(pos, &symtab->hlist_head[i], hlist) {
            symtab_user_cb(pos->key, pos->value, args);
        }
    }
}

