/**
 *
 * Release under GPLv2.
 * 
 * @file    input-key.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/03 2018 18:14
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/03 2018      create the file
 * 
 *     last modified: 30/03 2018 18:14
 */
#ifndef __BASE_LIB_INPUT_KEY_H_
#define __BASE_LIB_INPUT_KEY_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef INPUT_KEY_GB
#define INPUT_KEY_EX extern
#else
#define INPUT_KEY_EX
#endif

#define INPUT_DEV_PATH			"/dev/input"
#define MAX_INPUT_FD			(128)
#define INPUT_DEV_NAME_LEN		(128)

typedef struct {
	int fd[MAX_INPUT_FD];
	int len;
	char *name[MAX_INPUT_FD];
}input_fd_t;

typedef struct key_event {
	int key_type;
	int is_press;
}key_event_t;

#if 0
/*
 * The event structure itself
 */
struct input_event {
	struct timeval time;
	__u16 type;
	__u16 code;
	__s32 value;
};
#endif

INPUT_KEY_EX input_fd_t key_open(void);
INPUT_KEY_EX void key_close(input_fd_t *input_fd);

INPUT_KEY_EX void dump_key_event(key_event_t key_event);

INPUT_KEY_EX int _read_keys_event(int *fds, int len, key_event_t *key_event, int timeout_ms);
static inline int read_keys_event(int *fds, int len, key_event_t *key_event)
{
	return _read_keys_event(fds, len, key_event, 0);
}

static inline int read_keys_event_timeout(int *fds, int len, key_event_t *key_event, int timeout_ms)
{
	return _read_keys_event(fds, len, key_event, timeout_ms);
}

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_INPUT_KEY_H_ */

