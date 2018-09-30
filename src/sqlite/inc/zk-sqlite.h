/**
 *
 * Release under GPLv2.
 * 
 * @file    zk-sqlite.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/09 2018 17:11
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/09 2018      create the file
 * 
 *     last modified: 29/09 2018 17:11
 */
#ifndef __ZK_SQLITE_H_
#define __ZK_SQLITE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct zk_sqlite *pzk_sqlite_t;

typedef int (*zk_sqlite_cb_t)(void*, int, char**, char**);

#ifndef ZK_SQLITE_GB
#define ZK_SQLITE_EX extern
#else
#define ZK_SQLITE_EX
#endif

pzk_sqlite_t zk_sqlite_init(const char *db_path);

int zk_sqlite_exec(pzk_sqlite_t pzk_sqlite, 
        const char *sql, zk_sqlite_cb_t zk_sqlite_cb);

void zk_sqlite_clean(pzk_sqlite_t pzk_sqlite);

#ifdef __cplusplus
}
#endif

#endif

