/**
 * 
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/09 2018 17:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/09 2018      create the file
 * 
 *     last modified: 28/09 2018 17:20
 */
#include <stdio.h>

#include <utils/log_helper.h>

#include "zk-sqlite.h"

static int zk_sqlite_cb(void *NotUsed, int argc, char **argv, char **azColName)
{
   for (int i = 0; i < argc; i++) {
       log_i("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
   }

   log_i("");

   return 0;
}

int main(int argc, char const* argv[])
{
#define SQL_NUM     (3)
    char *sql_cmd[SQL_NUM] = {
        "CREATE TABLE COMPANY(" \
            "ID INT PRIMARY KEY     NOT NULL," \
            "NAME           TEXT    NOT NULL," \
            "AGE            INT     NOT NULL," \
            "ADDRESS        CHAR(50)," \
            "SALARY         REAL);",
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
            "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
            "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );",
        "SELECT * from COMPANY",
    };

    pzk_sqlite_t pzk_sqlite = zk_sqlite_init("./haha.db");

    for (int i = 0; i < SQL_NUM; i++) {
        log_i("-----------i: %d", i);
        zk_sqlite_exec(pzk_sqlite, sql_cmd[i], zk_sqlite_cb);
    }

    zk_sqlite_clean(pzk_sqlite);
    
    return 0;
}

