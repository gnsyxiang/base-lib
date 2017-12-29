/*
 *  Copyright (C) 2017 Ingenic Semiconductor
 *
 *  MaWeiBin <weibin.ma@ingenic.com>
 *
 *  Elf/IDWS Project
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifndef IPC_H
#define IPC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @note used IPC functions must add -lutils to LDFLAGS int Makefile
 */

struct IPC;

/**
 * IPC client
 */
typedef void (*ipc_client_callback_t)(struct IPC *client);

typedef void (*ipc_client_connect_change_t)(
        struct IPC *client, int is_connect);

struct ipc_client_callback {
    const char *what;
    ipc_client_callback_t cb;
};

enum {
    IPC_WRITE = 0xa5a5a5a5,
    IPC_WRITE_STRING,
    IPC_READ,
    IPC_READ_MALLOC,
    IPC_READ_STRING,
};

#define ipc_write_int(x)                    IPC_WRITE, sizeof(int), (int)(x)
#define ipc_write_types(x)                  IPC_WRITE, sizeof(x), (const void *)&(x)
#define ipc_write(buf, size)                IPC_WRITE, (int)(size), (const void *)(buf)
#define ipc_write_string(str)               IPC_WRITE_STRING, (const void *)(str)

#define ipc_read_types(x)                   IPC_READ, sizeof(x), (void *)&(x)
#define ipc_read(buf, size)                 IPC_READ, (int)(size), (void *)(buf)
#define ipc_read_malloc(ptr, size)          IPC_READ_MALLOC, (void **)&(ptr), (void *)&(size)
#define ipc_free_malloc(ptr)                free((void *)(ptr))
#define ipc_read_string(ptr)                IPC_READ_STRING, (void **)&(ptr)
#define ipc_free_string(ptr)                free((void *)(ptr))



/**
 * @brief get the ipc arguments
 * @code:
 *
 * int int_type;
 * char buf[100];
 *
 * ipc_get_args(ipc,
 *      ipc_types(int_type),
 *      ipc_buffer(buf, sizeof(buf)));
 */
#define ipc_get_args(ipc, ...)  _ipc_get_args(ipc, __VA_ARGS__, NULL)

/**
 * @brief return the ipc arguments
 *
 * @see @ipc_get_args
 */
#define ipc_return(ipc, ...)    _ipc_return(ipc, __VA_ARGS__, NULL)

/**
 * @brief connect to ipc server
 * @param server_name  server's name which connect to
 * @param tag  tag to debug
 * @param conn_chg  notify when connect status change
 * @param cb  client's callback called by ipc server,
 *            latst of cb array must to be set to {null, null}
 * @param private_data  user private data
 *
 * @return IPC client
 *
 * @see file IPC/ipc_example.h
 * @see connect_to_ipc_server_example
 */
struct IPC *connect_to_ipc_server(
        const char *server_name,
        const char *tag,
        ipc_client_connect_change_t conn_chg,
        struct ipc_client_callback cb[],
        long private_data);

/**
 * @brief disconnect to ipc server
 * @param client  ipc client
 */
void disconnect_to_ipc_server(struct IPC *client);

/**
 * @brief send to ipc server, will block until server return
 * @param client  ipc client
 * @param what  server callback's what
 * @param ...   server callback's args
 *
 * @return 0 if send success, otherwise return nagative
 *
 * @see file IPC/ipc_example.h
 * @see send_to_ipc_client_example
 *
 */
#define send_to_ipc_server(client, what, ...)       \
            _send_to_ipc_server(client, what, __VA_ARGS__, NULL)

/**
 * @brief get ipc client connect status
 * @param client  ipc client
 *
 * @return true if client is connecting
 */
int ipc_client_is_connect(struct IPC *client);

/**
 * IPC server
 */
typedef void (*ipc_server_callback_t)(
        struct IPC *server,
        struct IPC *client);

typedef void (*ipc_server_connect_change_t)(
        struct IPC *server,
        struct IPC *client, int is_connect);

struct ipc_server_callback {
    const char *what;
    ipc_server_callback_t cb;
};

/**
 * @brief create a ipc server
 * @param server_name  server's name
 * @param tag  tag to debug
 * @param conn_chg  notify when connect status change
 * @param cb  server's callback called by ipc client,
 *            latst of cb array must to be set to {null, null}
 * @param private_data  user private data
 *
 *
 * @return IPC server
 *
 * @see file IPC/ipc_example.h
 * @see create_ipc_server_example
 */
struct IPC *create_ipc_server(
        const char *server_name,
        const char *tag,
        ipc_server_connect_change_t conn_chg,
        struct ipc_server_callback cb[],
        long private_data);

/**
 * @brief delete ipc server
 * @param server  ipc server
 */
void delete_ipc_server(struct IPC *server);

/**
 * @brief send to ipc client, will block until client return
 * @param server ipc server
 * @param client  ipc client
 * @param what  client callback's what
 * @param ...   client callback's args
 *
 * @return 0 if send success, otherwise return nagative
 *
 * @see file IPC/ipc_example.h
 * @see send_to_ipc_client_example
 */
#define send_to_ipc_client(server, client, what, ...)   \
        _send_to_ipc_client(server, client, what, __VA_ARGS__, NULL)

/**
 * @brief get ipc server's clients
 * @param server  ipc server
 * @param clients connecting clients
 *
 * @return size of clients
 *
 * @note must call @put_ipc_server_clients after use
 *
 * @see put_ipc_server_clients
 * @see get_ipc_server_clients_example
 */
int get_ipc_server_clients(
        struct IPC *server, struct IPC *clients[128]);

/**
 * @brief put ipc server's clients
 * @param clients clients get by get_ipc_server_clients
 * @param size size of clients
 *
 * @see file IPC/ipc_example.h
 * @see get_ipc_server_clients_example
 */
void put_ipc_server_clients(
        struct IPC *clients[128], int size);

/**
 * IPC server and IPC client
 */

/**
 * @brief get ipc private data
 * @param IPC  ipc server or ipc client
 *          which get from @connect_to_ipc_server
 *          and @create_ipc_server
 *
 * @return user parvate data
 */
long get_ipc_private_data(struct IPC *IPC);

struct ipc_info {
    int pid;
    const char *name;
    const char *tag;
};

/**
 * @brief get ipc info
 * @param IPC  ipc to get info
 *          which get from @connect_to_ipc_server,
 *          @create_ipc_server and ipc callback
 *
 * @return ipc info
 */
struct ipc_info get_ipc_info(struct IPC *IPC);


void _ipc_get_args(struct IPC *ipc, ...);
void _ipc_return(struct IPC *ipc, ...);
int _send_to_ipc_client(
        struct IPC *server,
        struct IPC *client, const char *what, ...);
int _send_to_ipc_server(
        struct IPC *client, const char *what, ...);

#ifdef __cplusplus
}
#endif


#endif
