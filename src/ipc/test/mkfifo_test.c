/**
 * 
 * Release under GPLv2.
 * 
 * @file    fifowrite.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/01 2019 17:36
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/01 2019      create the file
 * 
 *     last modified: 12/01 2019 17:36
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h> 

#include <utils/log_helper.h>

#define FIFO_NAME   "/tmp/test_fifo"

int create_fifo(const char *path)
{
    if (access(path, F_OK) == -1) {
        int ret = mkfifo(path, 0777);
        if (ret != 0) {
            fprintf(stderr, "could not create fifo %s\n", path);
            return -1;
        }
    }

    int fifo_fd = open(path, O_WRONLY);
    if (fifo_fd == -1) {
        fprintf(stderr, "could not open fifo %d\n", fifo_fd);
        return -2;
    }

    return fifo_fd;
}

int open_fifo(const char *path)
{
    int fifo_fd = open(path, O_RDONLY);
    if (fifo_fd == -1) {
        fprintf(stderr, "could not open fifo %d\n", fifo_fd);
        return -1;
    }

    return fifo_fd;
}

int fifo_write(void)
{
    int ret = 0;
    char buffer[PIPE_BUF + 1];

    int fifo_fd = create_fifo(FIFO_NAME);

    int data_fd = open("res/demo.txt", O_RDONLY);
    if (data_fd == -1) {
        close(fifo_fd);
        fprintf (stderr, "--1--Open file[Data.txt] failed\n");
        return -3;
    }

    int bytes_read = read(data_fd, buffer, PIPE_BUF);
    buffer[bytes_read] = '\0';

    int write_sum = 0;
    while (bytes_read > 0) {
        ret = write(fifo_fd, buffer, bytes_read);
        if(ret == -1) {
            fprintf(stderr, "Write error on pipe\n");
            return -4;
        }

        write_sum += ret;

        bytes_read = read(data_fd, buffer, PIPE_BUF);
        buffer[bytes_read] = '\0';
    }

    close(fifo_fd);
    close(data_fd);

    return 0;
}

int fifo_read(void)
{
    char buffer[PIPE_BUF + 1];
    memset(buffer, '\0', sizeof(buffer));

    int fifo_fd = open_fifo(FIFO_NAME);

    int res = 0;

    while (1) {
        memset(buffer, '\0', sizeof(buffer));
        res = read(fifo_fd, buffer, PIPE_BUF);
        if (res <= 0)
            break;

        printf("--2--buffer: %s \n", buffer);
    }

    close(fifo_fd);

    return 0;
}

