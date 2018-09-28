/**
 * 
 * Release under GPLv2.
 * 
 * @file    ntp.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    27/09 2018 12:41
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        27/09 2018      create the file
 * 
 *     last modified: 27/09 2018 12:41
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <netdb.h>

#include "thread_helper.h"
#include "log_helper.h"
#include "socket.h"
#include "time_helper.h"

#define BASE_LIB_NTP_GB
#include "ntp.h"
#undef BASE_LIB_NTP_GB

#define NTP_HOSTNAME_LEN    (64)
#define DEFAULT_SYNC_TIME   (1 * 60)
#define DEFAULT_NTP_PORT    (123)

#define JAN_1970        0x83aa7e80
#define NTPFRAC(x)      (4294 * (x) + ((1981 * (x))>>11))
#define USEC(x)         (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))

typedef struct _ntp_status_t {
    struct timeval dly_time;
    struct timeval off_time;
    struct timeval new_time;

    int pfd[2];

    int ntp_loop_is_over;
    int ntp_loop_is_runnint;
} ntp_status_t;

typedef struct _ntp_usr_conf_t {
    char hostname[NTP_HOSTNAME_LEN];
    unsigned int sync_time_s;
}ntp_usr_conf_t;

static ntp_usr_conf_t ntp_usr_conf;
static ntp_status_t ntp_status;
static socket_t *client_sk = NULL;

static void mk_ntp_packet(unsigned int *packet)
{
    struct timeval now;
    gettimeofday(&now, NULL);

#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

    packet[0] = htonl((LI << 30) | (VN << 27) | (MODE << 24) 
            | (STRATUM << 16) | (POLL << 8) | (PREC & 0xff));
    packet[1] = htonl(1 << 16);
    packet[2] = htonl(1 << 16);

    packet[10] = htonl(now.tv_sec + JAN_1970);
    packet[11] = htonl(NTPFRAC(now.tv_usec));
}

struct ntp_time
{
	unsigned int time_s;
	unsigned int time_us;
};

static void parse_ntp_packet(unsigned int *packet)
{
    struct timeval now;
    gettimeofday(&now, NULL);

    struct ntp_time orig_time, recv_time, tras_time, dest_time;
    dest_time.time_s  = now.tv_sec + JAN_1970;
    dest_time.time_us = NTPFRAC(now.tv_usec);

#define DATA(i) ntohl(((unsigned int *)packet)[i])
    orig_time.time_s  = DATA(6);
    orig_time.time_us = DATA(7);

    recv_time.time_s  = DATA(8);
    recv_time.time_us = DATA(9);

    tras_time.time_s  = DATA(10);
    tras_time.time_us = DATA(11);
#undef DATA

    /*
     * Originate Timestamp      T1        客户端发送请求的时间
     * Receive Timestamp        T2        服务器接收请求的时间
     * Transmit Timestamp       T3        服务器答复时间
     * Destination Timestamp    T4        客户端接收答复的时间
     *
     * 网络延时d和服务器与客户端的时差t: 
     * d = (T2 - T1) + (T4 - T3); 
     * t = [(T2 - T1) + (T3 - T4)] / 2;
     */
#define MKSEC(ntpt) ((ntpt).time_s - JAN_1970)
#define MKUSEC(ntpt) (USEC((ntpt).time_us))
#define TTLUSEC(sec,usec) ((long long)(sec) * 1000000 + (usec))

#define GETSEC(us) ((us)/1000000)
#define GETUSEC(us) ((us)%1000000)

    struct timeval off_time, dly_time;
    unsigned long long orig_us,recv_us,tras_us,dest_us,off_us,dly_us;

    orig_us = TTLUSEC(MKSEC(orig_time), MKUSEC(orig_time));
    recv_us = TTLUSEC(MKSEC(recv_time), MKUSEC(recv_time));
    tras_us = TTLUSEC(MKSEC(tras_time), MKUSEC(tras_time));
    dest_us = TTLUSEC(now.tv_sec, now.tv_usec);

    off_us = ((recv_us - orig_us) + (tras_us - dest_us)) / 2;
    dly_us =  (recv_us - orig_us) + (dest_us - tras_us);

    off_time.tv_sec  = GETSEC (off_us);
    off_time.tv_usec = GETUSEC(off_us);

    dly_time.tv_sec  = GETSEC (dly_us);
    dly_time.tv_usec = GETUSEC(dly_us);

    struct timeval new_time;
    new_time.tv_sec  = dest_time.time_s - JAN_1970 + off_time.tv_sec;
    new_time.tv_usec = USEC(dest_time.time_us) + off_time.tv_usec;

    ntp_status.dly_time = dly_time;
    ntp_status.off_time = off_time;
    ntp_status.new_time = new_time;
}

static void send_packet(void)
{
    unsigned int packet[12] = {0};
    unsigned int packet_len = sizeof(packet);

    if (packet_len != 48) {
        log_e("ntp package size error");
        return;
    }

    mk_ntp_packet(packet);

    int ret = socket_udp_send_msg(client_sk, packet, packet_len);
    if (packet_len != ret) {
        log_e("socket_udp_send_msg error");
    }
}

static void disp_ntp_resp_time(const ntp_status_t * const ntp_status)
{
    log_i("----start: ");
    disp_timeval(&ntp_status->dly_time, "dly_time");
    disp_timeval(&ntp_status->off_time, "off_time");
    disp_timeval(&ntp_status->new_time, "new_time");
    log_i("-----------------------over\n");
}

static void recv_packet()
{
#define BUF_LEN     (1024)
    char buf[BUF_LEN];

    socket_udp_recv_msg(client_sk, buf, 1024);

    parse_ntp_packet((unsigned int *)&buf);

    disp_ntp_resp_time(&ntp_status);
}

static void update_localtime(const struct timeval *newtime)
{
    if (getuid() != 0 && geteuid() != 0){
        log_e( "不是 root 用户，无法进行时间校准，被迫终止!");
        exit(1);
    }

    if(settimeofday(newtime, NULL) == -1) {
        log_e("设置时间失败!");
    } else {
        log_i("设置时间成功!");
    }
}

static void ntp_update_notify(void)
{
    char ch;
    write(ntp_status.pfd[1], &ch, sizeof(ch));
}

void net_sync_time(void)
{
    send_packet();

    ntp_update_notify();
}

static void socket_cb(void *data)
{
    send_packet();

    while (ntp_status.ntp_loop_is_runnint) {
        recv_packet();

        update_localtime(&ntp_status.new_time);

        fd_set read_fs;
        FD_ZERO(&read_fs);
        FD_SET(ntp_status.pfd[0], &read_fs);

        int max_fd = ntp_status.pfd[0];
        struct timeval time_out;
        time_out.tv_sec  = ntp_usr_conf.sync_time_s;
        time_out.tv_usec = 0;

        int ret = select(max_fd+1, &read_fs, NULL, NULL, &time_out);
        if (ret < 0) {
            log_e("select error");
            continue;
        }

        if (FD_ISSET(ntp_status.pfd[0], &read_fs)) {
            char ch;
            read(ntp_status.pfd[0], &ch, sizeof(ch));
        }

        send_packet();
    }

    ntp_status.ntp_loop_is_over = 1;
}

static void *ntp_loop(void *data)
{
    char ip[INET_ADDRSTRLEN] = {0};

    hostname_to_ip(ntp_usr_conf.hostname, ip);

    client_sk = socket_udp_client_init(DEFAULT_NTP_PORT, ip);

    socket_connect(client_sk, socket_cb, 3, NULL);

    return NULL;
}

void ntp_init(const char *hostname, unsigned int sync_time_s)
{
    int hostname_len = strlen(hostname);
    if (!hostname || hostname_len + 1 >= NTP_HOSTNAME_LEN) {
        log_e("hostname is error ");
        return ;
    }

    if (0 == sync_time_s)
        sync_time_s = DEFAULT_SYNC_TIME;

    ntp_usr_conf.sync_time_s = sync_time_s;
    strncpy(ntp_usr_conf.hostname, hostname, hostname_len);

    ntp_status.ntp_loop_is_runnint = 1;
    ntp_status.ntp_loop_is_over    = 0;

    pipe(ntp_status.pfd);

    create_a_attached_thread(NULL, ntp_loop, NULL);

    log_i("ntp sync time is running");
}

void ntp_clean(void)
{
    ntp_status.ntp_loop_is_runnint = 0;

    while (!ntp_status.ntp_loop_is_over) {
        sleep(1);
    }

    log_i("ntp sync time is over");
}

