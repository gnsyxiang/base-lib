/**
 * 
 * Release under GPLv2.
 * 
 * @file    zk-sqlite.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/09 2018 10:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/09 2018      create the file
 * 
 *     last modified: 30/09 2018 10:08
 */
#include <stdio.h>

#include <sqlite3.h>
#include <utils/log_helper.h>
#include <utils/mem_helper.h>

#include "zk-sqlite.h"

#define DB_PATH_LEN     (256)

struct zk_sqlite {
    sqlite3 *db;
};
#define ZK_SQLITE_LEN   (sizeof(pzk_sqlite_t))

pzk_sqlite_t zk_sqlite_init(const char *db_path)
{
    if (!db_path) {
        log_e("the db path is NULL");
        return NULL;
    }

    pzk_sqlite_t pzk_sqlite = calloc(1, ZK_SQLITE_LEN);

    int ret = sqlite3_open(db_path, &pzk_sqlite->db);
    if (SQLITE_OK != ret) {
        log_e("sqlite3_open faild", sqlite3_errmsg(pzk_sqlite->db));
        return NULL;
    }

    return pzk_sqlite;
}

int zk_sqlite_exec(pzk_sqlite_t pzk_sqlite, 
        const char *sql, zk_sqlite_cb_t zk_sqlite_cb)
{
    char *err;

    int ret = sqlite3_exec(pzk_sqlite->db, sql, zk_sqlite_cb, 0, &err);
    if (ret != SQLITE_OK) {
        log_e("sql error: %s", err);
    }

    sqlite3_free(err);

    return ret;
}

void zk_sqlite_clean(pzk_sqlite_t pzk_sqlite)
{
    if (pzk_sqlite) {
        sqlite3_close(pzk_sqlite->db);

        free_mem(pzk_sqlite);
    }
}

