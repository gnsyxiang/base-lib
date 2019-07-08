/**
 * 
 * Release under GPLv2.
 * 
 * @file    uart_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/08 2019 17:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/08 2019      create the file
 * 
 *     last modified: 08/08 2019 17:08
 */
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#include "thread_wrapper.h"
#include "uart_wrapper.h"
#include "list.h"
#include "time_wrapper.h"
#include "file_wrapper.h"

typedef struct uart_state_tag {
    int fd;
    char *dev_path;
    speed_t speed;
    size_t select_timeout_ms;
    uart_read_cb_t uart_read_cb;

    int is_running;

    struct list_head list;
    pthread_mutex_t mutex;

    sem_t sem_write;
    sem_t sem_read_thread_exit;
    sem_t sem_write_thread_exit;
} uart_state_t;
#define UART_STATE_LEN (sizeof(uart_state_t))

typedef struct write_buf_tag {
    char *buf;
    int len;
    struct list_head list;
} write_buf_t;
#define WRITE_BUF_LEN (sizeof(write_buf_t))

#define READ_BUF_LEN   (16)

static char *g_uart_num_2_name[UART_NUM_MAX] = {
    "/dev/ttyUSB1",
    // "/dev/ttyS1",
    "/dev/ttyS2",
};

static speed_t g_speed_num_2_speed[UART_SPEED_MAX] = {
    B9600,
    B115200,
};

static uart_state_t *_uart_state_init(uart_config_t *uart_config)
{
    uart_state_t *uart_state = calloc(1, UART_STATE_LEN);
    if (!uart_state) {
        printf("calloc faild \n");
        return NULL;
    }

    uart_state->fd           = -1;
    uart_state->dev_path     = g_uart_num_2_name[uart_config->uart_num];
    uart_state->speed        = g_speed_num_2_speed[uart_config->speed];
    uart_state->is_running   = 1;
    uart_state->uart_read_cb = uart_config->uart_read_cb;
    uart_state->select_timeout_ms = uart_config->select_timeout_ms;

    sem_init(&uart_state->sem_write, 0, 0);
    sem_init(&uart_state->sem_write_thread_exit, 0, 0);
    sem_init(&uart_state->sem_read_thread_exit, 0, 0);
    INIT_LIST_HEAD(&uart_state->list);
    pthread_mutex_init(&uart_state->mutex, NULL);

    return uart_state;
}

static void _uart_state_final(uart_state_t *uart_state)
{
    pthread_mutex_destroy(&uart_state->mutex);
    sem_destroy(&uart_state->sem_read_thread_exit);
    sem_destroy(&uart_state->sem_write_thread_exit);
    sem_destroy(&uart_state->sem_write);

    free(uart_state);
}

static void _wait_sem(uart_state_t *uart_state)
{
    uart_state->is_running = 0;

    sem_wait(&uart_state->sem_read_thread_exit);
    sem_post(&uart_state->sem_write);
    sem_wait(&uart_state->sem_write_thread_exit);
}

static write_buf_t *_write_buf_init(void *buf, size_t len)
{
    write_buf_t *write_buf = calloc(1, WRITE_BUF_LEN);
    if (!write_buf) {
        printf("calloc faild \n");
        return NULL;
    }

    write_buf->buf = calloc(1, len);
    if (!write_buf->buf) {
        printf("calloc faild \n");
        return NULL;
    }

    memcpy(write_buf->buf, buf, len);
    write_buf->len = len;

    return write_buf;
}

static void _write_buf_final(write_buf_t *write_buf)
{
    free(write_buf->buf);
    free(write_buf);
}

static int _uart_write_common(void *handle, void *buf, size_t len, int flag)
{
    uart_state_t *uart_state = handle;
    write_buf_t *write_buf;

    if ((write_buf = _write_buf_init(buf, len)) == NULL) {
        printf("_write_buf_init faild \n");
        return -1;
    }

    if (flag) {
        pthread_mutex_lock(&uart_state->mutex);
        list_add_tail(&write_buf->list, &uart_state->list);
        pthread_mutex_unlock(&uart_state->mutex);
    } else {
        pthread_mutex_lock(&uart_state->mutex);
        list_add(&write_buf->list, &uart_state->list);
        pthread_mutex_unlock(&uart_state->mutex);
    }
    sem_post(&uart_state->sem_write);
    return len;
}

int uart_write(void *handle, void *buf, size_t len)
{
    if (!handle || !buf || len <= 0) {
        printf("the param is error \n");
        return -1;
    }

    return _uart_write_common(handle, buf, len, 0);
}

int uart_write_headbeat(void *handle, void *buf, size_t len)
{
    if (!handle || !buf || len <= 0) {
        printf("the param is error \n");
        return -1;
    }

    return _uart_write_common(handle, buf, len, 1);
}

void *uart_write_loop(void *args)
{
    uart_state_t *uart_state = args;
    write_buf_t *pos, *n;
    while (uart_state->is_running) {
        sem_wait(&uart_state->sem_write);
        pthread_mutex_lock(&uart_state->mutex);
        list_for_each_entry_safe(pos, n, &uart_state->list, list) {
            if (-1 == file_write(uart_state->fd, pos->buf, pos->len)) {
                printf("uart write internel faild \n");
                continue;
            }
            list_del(&pos->list);
            _write_buf_final(pos);
            break;
        }
        pthread_mutex_unlock(&uart_state->mutex);
    }
    sem_post(&uart_state->sem_write_thread_exit);
    return NULL;
}

void *uart_read_loop(void *args)
{
    char buf[READ_BUF_LEN];
    fd_set rfds;
    struct timeval time;
    uart_state_t *uart_state = args;
    int fd = uart_state->fd;
    int ret;

    while (uart_state->is_running) {
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        memset(buf, '\0', READ_BUF_LEN);
        time_ms_to_timeval(uart_state->select_timeout_ms, &time);

        switch (select(fd+1, &rfds, NULL, NULL, &time)) {
            case -1:
                printf("select error \n");
                break;
            case 0:
                printf("select timeout \n");
                break;
            default:
                ret = file_read(fd, buf, READ_BUF_LEN);
                if (uart_state->uart_read_cb) {
                    uart_state->uart_read_cb(buf, ret);
                }
                break;
        }
    }
    sem_post(&uart_state->sem_read_thread_exit);
    return NULL;
}

void _create_threads(uart_state_t *uart_state)
{
    create_a_attached_thread(NULL, uart_read_loop, uart_state);
    create_a_attached_thread(NULL, uart_write_loop, uart_state);
}

static inline void _set_speed_internel(struct termios *options, speed_t speed)
{
    cfsetispeed(options, speed);
    cfsetospeed(options, speed);
}

static inline void _set_option_internel(struct termios *options)
{
    cfmakeraw(options);          /* 配置为原始模式 */

    options->c_cflag &= ~CSIZE;
    options->c_cflag |= CS8;     /* 8位数据位 */
    options->c_cflag &= ~PARENB;  /* enable parity */
    //options->c_cflag &= ~PARODD; /* 偶校验 */
    options->c_iflag |= INPCK;   /* disable parity checking */
    options->c_cflag &= ~CSTOPB; /* 一个停止位 */
    options->c_cc[VTIME] = 0;    /*设置等待时间*/
    options->c_cc[VMIN] = 0;     /*最小接收字符*/
}

static int _set_param_common(uart_state_t *uart_state, int flag)
{
    struct termios options;
    int fd = uart_state->fd;
    speed_t speed = uart_state->speed;

    if (tcgetattr(fd, &options) != 0) {
        printf("tcgetarrt faild \n");
        return -1;
    }

    if (flag) {
        _set_option_internel(&options);
    } else {
        _set_speed_internel(&options, speed);
    }

    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        printf("tcsetarrt faild \n");
        return -1;
    }
    tcflush(fd, TCIOFLUSH);

    return 0;
}

#define _set_parity(uart_state) _set_param_common(uart_state, 1)
#define _set_speed(uart_state) _set_param_common(uart_state, 0)

#define _set_uart_init()                                        \
    do {                                                        \
        int flags = O_RDWR | O_NOCTTY | O_NDELAY;               \
        uart_state->fd = open(uart_state->dev_path, flags);     \
        if (uart_state->fd <= 0) {                              \
            printf("open %s faild \n", uart_state->dev_path);   \
            goto L_OPEN_FAILD;                                  \
        }                                                       \
        \
        if (0 != _set_speed(uart_state)) {                      \
            printf("set speed  faild \n");                      \
            goto L_SET_SPEED_FAILD;                             \
        }                                                       \
        \
        if (0 != _set_parity(uart_state)) {                     \
            printf("set parity faild \n");                      \
            goto L_SET_SPEED_FAILD;                             \
        }                                                       \
    } while(0)

#define _set_uart_final()                                       \
    do {                                                        \
        if (0 != close(uart_state->fd)) {                       \
            printf("close faild \n");                           \
            return -1;                                          \
        }                                                       \
    } while(0)

void *uart_init(uart_config_t *uart_config)
{
    if (!uart_config) {
        printf("the param is NULL \n");
        return NULL;
    }

    uart_state_t *uart_state = _uart_state_init(uart_config);
    if (!uart_state) {
        printf("_uart_state_init faild \n");
        return NULL;
    }

    _set_uart_init();
    _create_threads(uart_state);

    printf("uart init successful \n");

    return ((void *)uart_state);

L_SET_SPEED_FAILD:
    close(uart_state->fd);
L_OPEN_FAILD:
    free(uart_state);
    return NULL;
}

int uart_final(void *handle)
{
    if (!handle) {
        printf("the param is NULL \n");
        return -1;
    }

    uart_state_t *uart_state = handle;

    _wait_sem(uart_state);
    _set_uart_final();
    _uart_state_final(uart_state);

    return 0;
}

