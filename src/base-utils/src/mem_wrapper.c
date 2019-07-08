/**
 * 
 * Release under GPLv2.
 * 
 * @file    mem_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/07 2019 20:09
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/07 2019      create the file
 * 
 *     last modified: 23/07 2019 20:09
 */
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

#include "symtab.h"
#include "log_wrapper.h"

#define BASE_LIB_UTILS_SRC_MEM_WRAPPER_GB
#include "mem_wrapper.h"
#undef BASE_LIB_UTILS_SRC_MEM_WRAPPER_GB

#define LOG_MEM_TAG     "mem"
#define BUF_LEN         (256)
#define MEM_LEAK_NUM    (32)
#define POINT_LEN       (32)

typedef struct mem_leak_tag {
    int init_flag;
    pthread_mutex_t mutex;
    void *symtab_handle;
} mem_leak_t;

static mem_leak_t g_mem_leak;

static int _mem_init(void)
{
    int ret = 0;

    ret += pthread_mutex_init(&g_mem_leak.mutex, NULL);
    ret += symtab_init(&g_mem_leak.symtab_handle, MEM_LEAK_NUM);

    if (ret != 0) {
        return -1;
    }

    g_mem_leak.init_flag = 1;

    return ret;
}

void *_mem_calloc(const char *file, const char *func, size_t line, int num, size_t size)
{
    assert(size != 0);

    if (!g_mem_leak.init_flag && _mem_init() != 0) {
        loge(LOG_MEM_TAG, "call _mem_init faild \n");
        return NULL;
    }

    void *p = calloc(num, size);
    if (!p) {
        loge(LOG_MEM_TAG, "calloc faild\n");
        return NULL;
    }

    char p_2_str[POINT_LEN] = {0};
    char buf[BUF_LEN] = {0};
    snprintf(buf, BUF_LEN, "file[%s], func[%s], line[%ld]", file, func, line);
    snprintf(p_2_str, POINT_LEN, "%p", p);

    pthread_mutex_lock(&g_mem_leak.mutex);
    symtab_insert(g_mem_leak.symtab_handle, p_2_str, buf);
    pthread_mutex_unlock(&g_mem_leak.mutex);

    return p;
}

void _mem_free(const char *file, const char *func, size_t line, void **p)
{
    assert(p != NULL);
    assert(*p != NULL);
    free(*p);

    char p_2_str[POINT_LEN] = {0};
    snprintf(p_2_str, POINT_LEN, "%p", *p);

    pthread_mutex_lock(&g_mem_leak.mutex);
    symtab_remove(g_mem_leak.symtab_handle, p_2_str);
    pthread_mutex_unlock(&g_mem_leak.mutex);

    *p = NULL;
}

void mem_user_cb(char *key, void *value, void *args)
{
    loge(LOG_MEM_TAG, "key: %s, value: %s \n", key, (char *)value);
}

static void _mem_final(void)
{
    pthread_mutex_destroy(&g_mem_leak.mutex);
    symtab_final(g_mem_leak.symtab_handle);
    g_mem_leak.init_flag = 0;
}

void mem_leak_display(void)
{
    symtab_map_user_display(g_mem_leak.symtab_handle, mem_user_cb, NULL);
    _mem_final();
}

