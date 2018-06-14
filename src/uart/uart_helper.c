/**
 * 
 * Release under GPLv2.
 * 
 * @file    uart_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/06 2018 10:02
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/06 2018      create the file
 * 
 *     last modified: 14/06 2018 10:02
 */
#include <stdio.h>
#include <termios.h>

#define UART_HELPER_GB
#include "uart_helper.h"
#undef UART_HELPER_GB

int get_termios(int fd, struct termios *s)
{
	return tcgetattr(fd, s);
}
int set_termios(int fd, const struct termios *s)
{
	return tcsetattr(fd, TCSANOW, s);
}

void enable_read(struct termios *s)
{
    s->c_cflag |= CREAD;
}
void disable_read(struct termios *s)
{
    s->c_cflag &= ~CREAD;
}
int enable_flow_control(struct termios *s)
{
    s->c_cflag |= CRTSCTS;
}
int disable_flow_control(struct termios *s)
{
    s->c_cflag &= ~CRTSCTS;
}

static int baud_to_speed(int baud)
{
    switch(baud) {
        case B1200:     return 1200;
        case B2400:     return 2400;
        case B4800:     return 4800;
        case B9600:     return 9600;
        case B19200:    return 19200;
        case B38400:    return 38400;
        case B57600:    return 57600;
        case B115200:   return 115200;
    }

    return 0;
}

int get_speed(const struct termios *s)
{
    return baud_to_speed(s->c_cflag & CBAUD);
}
int get_ispeed(const struct termios *s)
{
    return baud_to_speed(s->c_iflag & CBAUD);
}
int get_ospeed(const struct termios *s)
{
    return baud_to_speed(s->c_oflag & CBAUD);
}

static int speed_to_baud(int speed)
{
	switch(speed) {
		case 1200:      return B1200;
		case 2400:      return B2400;
		case 4800:      return B4800;
		case 9600:      return B9600;
		case 19200:     return B19200;
		case 38400:     return B38400;
		case 57600:     return B57600;
		case 115200:    return B115200;
	}
	return B9600;
}

static inline void set_speed(struct termios *s, int speed)
{
    s->c_cflag &= ~CBAUD;
    s->c_cflag |= speed_to_baud(speed);
}
int set_ispeed(struct termios *s, int speed)
{
    return cfsetispeed(s, speed_to_baud(speed) );
}
int set_ospeed(struct termios *s, int speed)
{
    return cfsetospeed(s, speed_to_baud(speed) );
}

int get_bsize(const struct termios *s)
{
    switch(s->c_cflag & CSIZE) {
        case CS5: return 5;
        case CS6: return 6;
        case CS7: return 7;
        case CS8: return 8;
    }

    return -1;
}
static inline void set_bsize(struct termios *s, int bsize)
{
    s->c_cflag &= ~CSIZE;
    switch(bsize) {
        case 5: s->c_cflag |= CS5; break;
        case 6: s->c_cflag |= CS6; break;
        case 7: s->c_cflag |= CS7; break;
        case 8: s->c_cflag |= CS8; break;
    }
}

char get_parity(const struct termios *s)
{
    if(s->c_cflag & PARENB) {
        if(s->c_cflag & PARODD)
            return 'O';
        else
            return 'E';
    }

    return 'N';
}
static inline void set_parity(struct termios *s, char parity)
{
    switch(parity) {
        case 'n':
        case 'N':
            s->c_cflag &= ~PARENB;
            break;
        case 'o':
        case 'O':
            s->c_cflag |= PARENB;
            s->c_cflag |= PARODD;
            break;
        case 'e':
        case 'E':
            s->c_cflag |= PARENB;
            s->c_cflag &= ~PARODD;
            break;
    }
}

int get_stop(const struct termios *s)
{
    if(s->c_cflag & CSTOPB)
        return 2;
    return 1;
}
static inline void set_stop(struct termios *s, int stop)
{
    if( 1 == stop )
        s->c_cflag &= ~CSTOPB;
    else
        s->c_cflag |= CSTOPB;
}

int uart_init(int fd, int baud, char parity, int bsize, int stop)
{
    struct termios new = {0};

    set_speed(&new, baud);

    set_bsize(&new, bsize);
    set_parity(&new, parity);
    set_stop(&new, stop);

    enable_read(&new);
    disable_flow_control(&new);

    if(set_termios(fd, &new))
    {
        return -6;
    }

    return 0;
}

#ifdef UART_DEBUG
void show_termios(unsigned int fd, const struct termios *s)
{
    struct termios old = {0};

    get_termios(fd, &old);

    printf("speed=%d\n", get_speed(s));
    printf("parity=%c\n", get_parity(s));
    printf("bsize=%d\n", get_bsize(s));
    printf("stop=%d\n", get_stop(s));
}
#endif

