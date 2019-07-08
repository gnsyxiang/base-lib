/**
 * 
 * Release under GPLv2.
 * 
 * @file    uart_protocol.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/08 2019 12:18
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/08 2019      create the file
 * 
 *     last modified: 09/08 2019 12:18
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

#include "uart_wrapper.h"
#include "thread_wrapper.h"

#define BASE_LIB_UTILS_SRC_UART_PROTOCOL_GB
#include "uart_protocol.h"
#undef BASE_LIB_UTILS_SRC_UART_PROTOCOL_GB

typedef struct frame_head_tag {
    unsigned short head;
    int cmd;
    unsigned short bc;
} UNPACK frame_head_t;
#define FRAME_HEAD_LEN (sizeof(frame_head_t))

typedef struct uart_protocol_tag {
    void *uart1_handle;
    sem_t sem_thread_exit;

    int is_heatbeat_running;
    size_t heatbeat_timeout_ms;
    uart_protocol_read_cb_t uart_protocol_read_cb;
} uart_protocol_t;
#define UART_PROTOCOL_LEN (sizeof(uart_protocol_t))

#define HEAD_ID     (0xbb00)
#define UART_PROTOCOL_READ_BUF_LEN   (32)
#define UART_PROTOCOL_WRITE_BUF_LEN   (128)

typedef unsigned short uint_16;
#define BSWAP_16(x) \
    (uint_16)((((uint_16)(x) & 0x00ff) << 8) | \
            (((uint_16)(x) & 0xff00) >> 8) \
            )

#define right_shift(dst, cmd, len)              \
    ({                                          \
        int i;                                  \
        char *tmp = dst;                        \
        for (i = len - 1; i >= 0; i--) {        \
            *tmp++ = (cmd >> (i * 8) & 0xff);   \
        }                                       \
        (len);                                  \
    })

static uart_protocol_t *g_uart_protocol;

static inline unsigned int _set_protocol_head(char *dst)
{
    return right_shift(dst, HEAD_ID, 2);
}

static inline unsigned int _set_protocol_frame(frame_t *frame, char *dst)
{
    unsigned int cmd = frame->cmd;
    unsigned short bc = frame->len;
    int len = 0;
    len += right_shift(dst + len, cmd, 4);
    len += right_shift(dst + len, bc, 2);

    if (!frame->data) {
        return len;
    } else {
        memcpy(dst + len, frame->data, frame->len);
        free(frame->data);
        return (len + bc);
    }
}

static unsigned int _set_protocol_fcc(char *dst, unsigned int len)
{
    unsigned short fcc = 0;
    for (int i = 0; i < len; i++) {
        fcc += dst[i];
    }
    *(dst + len) = fcc;

    return 2;
}

static int _protocol_code(frame_t *frame, char *dst)
{
    char *tmp = dst;

    tmp += _set_protocol_head(tmp);
    tmp += _set_protocol_frame(frame, tmp);
    tmp += _set_protocol_fcc(dst, tmp - dst);

    return (tmp - dst);
}

int uart_protocol_write(void *frame)
{
    if (!frame) {
        printf("the param is error \n");
        return -1;
    }

    char buf[UART_PROTOCOL_WRITE_BUF_LEN] = {0};
    int ret = _protocol_code(frame, buf);
    return uart_write(g_uart_protocol->uart1_handle, buf, ret);
}

static frame_t *_frame_init(frame_head_t *frame_head, unsigned char *src)
{
    frame_t *frame = calloc(1, FRAME_LEN);
    if (!frame) {
        printf("calloc faild \n");
        return NULL;
    }
    frame->data = calloc(1, frame_head->bc);
    if (!frame->data) {
        printf("calloc faild \n");
        return NULL;
    }
    frame->cmd = frame_head->cmd;
    frame->len = frame_head->bc;
    int bc = frame_head->bc;
    if (bc) {
        memcpy(frame->data, src + FRAME_HEAD_LEN, bc);
    }

    return frame;
}

static unsigned short _check_protocol_fcc(unsigned char *src, int len)
{
    int fcc = 0;
    for (int i = 0; i < len; i++) {
        fcc += src[i];
    }
    return fcc;
}

static frame_t *_protocol_decode(unsigned char *src, int len)
{
#define SRC_TMP_LEN (128)
    static unsigned char src_tmp[SRC_TMP_LEN] = {0};
    static int src_tmp_len = 0;
    static int find_head_flag = 0;

    static frame_head_t *frame_head;
    int offset_fcc;
    int frame_len;
    int copy_flag = 0;

    while (len) {
        if (!copy_flag) {
            memcpy(src_tmp + src_tmp_len, src, len);
            src_tmp_len += len;
            copy_flag = 1;
        }

        if (src_tmp_len < sizeof(frame_head_t)) {
            break;
        }

        if (!find_head_flag) {
            frame_head = (frame_head_t *)src_tmp;
            if (BSWAP_16(frame_head->head) != HEAD_ID) {
                memcpy(src_tmp, src_tmp + 1, src_tmp_len);
                src_tmp_len--;
                len--;
                continue;
            }
            find_head_flag = 1;
        }

        frame_head = (frame_head_t *)src_tmp;
        offset_fcc = frame_head->bc + FRAME_HEAD_LEN;
        frame_len = offset_fcc + 2;
        if (src_tmp_len < frame_len) {
            break;
        }

        if (src_tmp[offset_fcc] == _check_protocol_fcc(src_tmp, offset_fcc)) {
            frame_t *frame = _frame_init(frame_head, src_tmp);
            memcpy(src_tmp, src_tmp + frame_len, src_tmp_len - frame_len);
            src_tmp_len -= frame_len;
            find_head_flag = 0;
            return frame;
        }
    }
    //FIXME
    return NULL;
}

void uart_read_cb(void *buf, size_t len)
{
    frame_t *frame = _protocol_decode(buf, len);

    if (g_uart_protocol->uart_protocol_read_cb && frame) {
        g_uart_protocol->uart_protocol_read_cb(frame);
    }
}

int _uart_protocol_beathead_write_internel(void)
{
    char buf[UART_PROTOCOL_WRITE_BUF_LEN] = {0};
    frame_t frame;
    frame.cmd = 0x02;
    frame.len = 0;
    frame.data = NULL;
    int ret = _protocol_code(&frame, buf);
    return uart_write_headbeat(g_uart_protocol->uart1_handle, buf, ret);
}

static void *uart_protocol_beatheat_loop(void *args)
{
    struct timeval time;
    while (g_uart_protocol->is_heatbeat_running) {
        time.tv_sec = g_uart_protocol->heatbeat_timeout_ms / 1000;
        time.tv_usec = g_uart_protocol->heatbeat_timeout_ms % 1000 * 1000;
        select(0, NULL, NULL, NULL, &time);

        _uart_protocol_beathead_write_internel();
    }
    sem_post(&g_uart_protocol->sem_thread_exit);
    return NULL;
}

int uart_protocol_init(uart_protocol_config_t *uart_protocol_config)
{
    if (!uart_protocol_config) {
        printf("the param is error \n");
        goto L_ERROR;
    }

    g_uart_protocol = calloc(1, UART_PROTOCOL_LEN);
    if (!g_uart_protocol) {
        printf("calloc is faild \n");
        goto L_ERROR;
    }

    uart_config_t uart_config;
    uart_config.uart_num          = UART_NUM_1;
    uart_config.speed             = UART_SPEED_9600;
    uart_config.uart_read_cb      = uart_read_cb;
    uart_config.select_timeout_ms = 1000;

    g_uart_protocol->uart1_handle = uart_init(&uart_config);
    if (!g_uart_protocol->uart1_handle) {
        printf("uart_init faild \n");
        goto L_UART_INIT_FAILD;
    }

    if (uart_protocol_config->uart_protocol_read_cb) {
        g_uart_protocol->uart_protocol_read_cb = uart_protocol_config->uart_protocol_read_cb;
    }

    if (uart_protocol_config->heatbeat_flag) {
        g_uart_protocol->is_heatbeat_running = uart_protocol_config->heatbeat_flag;
        g_uart_protocol->heatbeat_timeout_ms = uart_protocol_config->heatbeat_timeout_ms;
        sem_init(&g_uart_protocol->sem_thread_exit, 0, 0);
        create_a_attached_thread(NULL, uart_protocol_beatheat_loop, NULL);
    }

    return 0;
L_UART_INIT_FAILD:
    free(g_uart_protocol);
L_ERROR:
    return -1;
}

void uart_protocol_final(void)
{
    g_uart_protocol->is_heatbeat_running = 0;

    sem_wait(&g_uart_protocol->sem_thread_exit);
    sem_destroy(&g_uart_protocol->sem_thread_exit);

    uart_final(g_uart_protocol->uart1_handle);
    free(g_uart_protocol);
}

