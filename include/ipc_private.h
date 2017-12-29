#ifndef IPC_PRIVATE_H
#define IPC_PRIVATE_H

#include <stdarg.h>
#include <assert.h>

#include "ipc.h"
#include "list.h"
#include "log_helper.h"

#pragma GCC visibility push(hidden)

struct link;

enum {
    IPC_CLIENT,
    IPC_SERVER,
    IPC_LINK,
};

struct ipc {
    int type;
    char *name;
    char *tag;
    int pid;

    long private_data;

    int is_running;
    pthread_t tid;
    pthread_mutex_t lock;
    struct list_head func_list;

    /**
     * store every callback
     */
    struct list_head call_list;

    struct list_head out_stream_list;
    struct list_head in_stream_list;
    pthread_mutex_t stream_list_lock;

    int used_count;

    union {
        /**
         * type == IPC_CLIENT for client
         */
        struct {
            struct link *link;
            ipc_client_connect_change_t on_client_connect_change;
        };

        /**
         * type == IPC_SERVER for server
         */
        struct {
            /**
             * pipe fd for notify new link
             */
            int pfd[2];
            struct link_manager *link_manager;
            ipc_server_connect_change_t on_server_connect_change;
        };
    };
};

struct ipc *create_ipc(
        const char *name,
        const char *tag, long private_data);

void disconnect_ipc(struct ipc *ipc);
void delete_ipc(struct ipc *ipc);

void ipc_lock(struct ipc *ipc);
void ipc_unlock(struct ipc *ipc);

typedef enum result_t {
    RET_NO_ERROR,
    RET_IO_ERROR,
    RET_MSG_ERROR,
    RET_REMOTE_DISCONNECT,
}result_t;

result_t analyze_ipc_message(
        struct ipc *ipc, struct link *link);

int send_to_ipc(
        struct ipc *ipc, struct link *link,
        const char *what);

result_t send_ipc_info(struct ipc *ipc, struct link *link);

void start_ipc_connect_change_thread(
        struct ipc *ipc,
        struct link *link, int is_connect);

void register_ipc_callback(
        struct ipc *ipc, const char *what, void *cb);

struct ipc_info get_ipc_server_info(struct IPC *server);
struct ipc_info get_ipc_client_info(struct IPC *client);

void ipc_delete_stream(struct ipc *ipc);
void ipc_in_args_parse(struct ipc *ipc, va_list args);
void ipc_out_args_parse(struct ipc *ipc, va_list args);

int ipc_peek_in_arg_size(struct ipc *ipc);

#pragma GCC visibility pop

#endif
