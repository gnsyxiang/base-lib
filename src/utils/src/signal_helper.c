/**
 * 
 * Release under GPLv2.
 * 
 * @file    signal_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/03 2018 16:05
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/03 2018      create the file
 * 
 *     last modified: 02/03 2018 16:05
 */
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <linux/sockios.h>
#include <linux/input.h>
#include <math.h>
#include <ctype.h>

#define UTILS_SIGNAL_GB
#include "signal_helper.h"
#undef UTILS_SIGNAL_GB

static char *global_app_name;

static char *signal_str[] = {
    [1] = "SIGHUP",       [2] = "SIGINT",       [3] = "SIGQUIT",      [4] = "SIGILL",
    [5] = "SIGTRAP",      [6] = "SIGABRT",      [7] = "",             [8] = "SIGFPE",
    [9] = "SIGKILL",      [10] = "SIGUBS",      [11] = "SIGSEGV",     [12] = "SIGSYS",
    [13] = "SIGPIPE",     [14] = "SIGALRM",     [15] = "SIGTERM",     [16] = "SIGUSR1",
    [17] = "SIGUSR2",     [18] = "SIGCHLD",     [19] = "SIGPWR",      [20] = "SIGWINCH",
    [21] = "SIGURG",      [22] = "SIGPOLL",     [23] = "SIGSTOP",     [24] = "SIGTSTP",
    [25] = "SIGCONT",     [26] = "SIGTTIN",     [27] = "SIGTTOU",     [28] = "SIGVTALRM",
    [29] = "SIGPROF",     [30] = "SIGXCPU",     [31] = "SIGXFSZ",
};

static void sig_handler(int signo)
{
    char cmd[256] = {0};
    void *array[10];
    int size = 0;
    char **strings = NULL;
    int i = 0;

    printf("\n\n[%s] %s(%d) crashed by signal %s.\n",
           __func__, global_app_name, getpid(), signal_str[signo]);

    printf("Call Trace:\n");
#ifndef NO_backtrace
    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);
#endif
    if (strings) {
        for (i = 0; i < size; i++)
            printf("  %s\n", strings[i]);
        free(strings);
    } else {
        printf("Not Found\n\n");
    }

    if (signo == SIGINT || signo == SIGUSR1 || signo == SIGUSR2) {
        exit(-1);
    } else {
        system("mkdir -p /mnt/sdcard/coredump");
        sprintf(cmd, "cat /proc/%d/maps", getpid());
        printf("Process maps:\n");
        system(cmd);

        snprintf(cmd, 256, "cat /proc/%d/maps > /mnt/sdcard/coredump/%s.%d.maps",
             getpid(), global_app_name, getpid());
        system(cmd);
    }
}

void register_linux_signal_hanler(const char *app_name)
{
    static struct sigaction act;

    global_app_name = strdup(app_name);

    act.sa_flags = SA_RESETHAND;
    sigemptyset(&act.sa_mask);
    act.sa_handler = sig_handler;

    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGILL,  &act, NULL);
    sigaction(SIGTRAP, &act, NULL);
    sigaction(SIGABRT, &act, NULL);
    sigaction(SIGFPE,  &act, NULL);
    sigaction(SIGSEGV, &act, NULL);
    sigaction(SIGBUS,  &act, NULL);
    sigaction(SIGSYS,  &act, NULL);
    sigaction(SIGXCPU, &act, NULL);
    sigaction(SIGXFSZ, &act, NULL);

    sigaction(SIGINT,  &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);

    signal(SIGPIPE, SIG_IGN);
}

