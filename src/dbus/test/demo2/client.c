#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void method_call(DBusConnection* conn)
{
    DBusPendingCall* Pending;
    DBusMessageIter args;
    dbus_bool_t nRet = FALSE;
    const char *v_STRING = "method_call";


    DBusMessage *message = dbus_message_new_method_call(
            SERVER_WELL_NAME,
            SERVER_OBJ_PATH,
            SERVER_INTERFACE,
            SERVER_METHOD);
    if (!message) {
        fprintf(stderr, "Message NULL\n");
        return;
    }

    nRet = dbus_message_append_args(
            message,
            DBUS_TYPE_STRING,
            &v_STRING,
            DBUS_TYPE_INVALID);

    dbus_connection_send_with_reply(conn, message, &Pending, -1);
    if (FALSE == nRet) {
        fprintf(stderr, "(send)Out of Memeory\n");
        return;
    }
    /*
       if(FALSE == dbus_pending_call_set_notify(
       Pending, method_call_notify, NULL, NULL))
       {
       printf("OMM\n");
       }
       dbus_connection_flush(conn);
       dbus_message_unref(message);
       message = NULL;


       if(TRUE == dbus_pending_call_get_completed(Pending))
       {
       printf("complete\n");
       }

       dbus_pending_call_unref(Pending);
       */

    dbus_connection_flush(conn);
    dbus_message_unref(message);
    message = NULL;

    dbus_pending_call_block(Pending);
    message = dbus_pending_call_steal_reply(Pending);
    if (!message) {
        fprintf(stderr, "Reply Null\n");
        exit(1);
    }

    // free the pending message handle
    dbus_pending_call_unref(Pending);

    char* level = NULL;
    // read the parameters
    if (!dbus_message_iter_init(message, &args))
        fprintf(stderr, "Message has no arguments!\n");
    else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "Argument is not int!\n");
    else
        dbus_message_iter_get_basic(&args, &level);

    printf("Got Reply: %s\n", level);

    dbus_message_unref(message);
}

static void emit_signal(DBusConnection *dbconn)
{
    DBusMessage *dbmsg = dbus_message_new_signal(
            "/client/signal/Object",    // object name of the signal
            "client.signal.Type",       // interface name of the signal
            "Test"                      // name of the signal
            );                   
    if (!dbmsg)
        return;

    char *word = "hello world";
    if (!dbus_message_append_args(
                dbmsg,
                DBUS_TYPE_STRING,
                &word,
                DBUS_TYPE_INVALID
                )) {
        return;
    }

    dbus_uint32_t serial;
    if (!dbus_connection_send(dbconn, dbmsg, &serial)) {
        return;
    }

    dbus_connection_flush(dbconn);

    dbus_message_unref(dbmsg);
    dbmsg = NULL;

    printf("send message %s, serial: %d \n", word, serial);
}

int main(int argc, char *argv[])
{
    DBusError dberr;

    dbus_error_init(&dberr);

    //连接到dbus,建立一个连接，称为对象
    DBusConnection *dbconn = dbus_bus_get(DBUS_BUS_SESSION, &dberr);
    if (!dbconn && dbus_error_is_set(&dberr)) {
        goto err_1;
    }

    //设置为当收到disconnect信号的时候不退出应用程序(_exit())
    // dbus_connection_set_exit_on_disconnect(dbconn, FALSE);
    //

    //请求获取公共名
    // int ret = dbus_bus_request_name(dbconn,
    // CLIENT_WELL_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &dberr);
    // if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
    // printf("request name failed \n");
    // goto err_2;
    // }

    emit_signal(dbconn);

    dbus_error_free(&dberr);
    dbus_connection_unref(dbconn);

    return 0;

err_2:
    dbus_connection_unref(dbconn);

err_1:
    dbus_error_free(&dberr);

    return -1;
}

