/**
 * 
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/01 2019 17:28
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/01 2019      create the file
 * 
 *     last modified: 12/01 2019 17:28
 */
#include <stdio.h>
#include <stdlib.h>

int fifo_write(void);
int fifo_read(void);

int main(int argc, const char *argv[])
{
    if (atoi(argv[1]) == 1)
        fifo_write();
    else
        fifo_read();

    return 0;
}
