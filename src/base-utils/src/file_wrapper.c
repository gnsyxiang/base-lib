/**
 * 
 * Release under GPLv2.
 * 
 * @file    file_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/08 2019 15:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/08 2019      create the file
 * 
 *     last modified: 12/08 2019 15:20
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>

#include "log_wrapper.h"
#include "time_wrapper.h"

#define BASE_LIB_UTILS_SRC_FILE_WRAPPER_GB
#include "file_wrapper.h"
#undef BASE_LIB_UTILS_SRC_FILE_WRAPPER_GB

#define FILE_WRAPPER_TAG "file_wrapper"

/**
 * @brief get the length of the file
 *
 * @param path: file name
 *
 * @return: successful, return total length of a file; faild, return -1 
 */
long open_file_get_len(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (NULL == fp) {
        loge(FILE_WRAPPER_TAG, "fopen %s faild", path);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    if (len <= 0) {
        printf("threre is no data in the %s file \n", path);
        return -1;
    }

    fclose(fp);

    return len;
}

static int _set_fcntl(int fd, long arg)
{
    int flags;

    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
        flags = 0;

    return fcntl(fd, F_SETFL, flags | arg);
}

int file_set_nonblocking(int fd)
{
    return _set_fcntl(fd, O_NONBLOCK);
}

/*
 * 作用: 当fork子进程后，仍然可以使用fd。
 *       但执行exec后系统就会自动关闭进程中的fd
 */
int file_close_on_exec(int fd)
{
    return _set_fcntl(fd, FD_CLOEXEC);
}

ssize_t file_write(int fd, const void *buf, size_t cnt)
{
    int ret;
    size_t nleft;
    const char *ptr;

    ptr = buf;
    nleft = cnt;

    while (nleft > 0) {
        if ((ret = write(fd, ptr, nleft)) <= 0) {
            if (ret < 0 && errno == EINTR)
                ret = 0;
            else
                return -1;
        }

        nleft -= ret;
        ptr   += ret;
    }

    return cnt;
}

ssize_t file_read(int fd, void *buf, size_t cnt)
{
    int ret;
    size_t nleft;
    size_t offset = 0;

    nleft = cnt;

    while (nleft > 0) {
        if ((ret = read(fd, buf + offset, nleft)) < 0) {
            if (errno == EINTR)
                ret = 0;
            else
                return -1;
        } else if (ret == 0)
            break;

        nleft  -= ret;
        offset += ret;
    }

    return offset;
}

ssize_t file_read_timeout(int fd, void *buf, size_t cnt, size_t timeout_ms)
{
    fd_set rfds;
    struct timeval time;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    time_ms_to_timeval(timeout_ms, &time);

    size_t ret = select(fd+1, &rfds, NULL, NULL, &time);
    switch (ret) {
        case -1:
            loge(FILE_WRAPPER_TAG, "select error");
            cnt = -1;
            break;
        case 0:
            loge(FILE_WRAPPER_TAG, "select timeout");
            cnt = -1;
            break;
        default:
            if ((cnt = file_read(fd, buf, cnt)) == -1) {
                loge(FILE_WRAPPER_TAG, "read error");
                cnt = -1;
            }
            break;
    }

    return cnt;
}

