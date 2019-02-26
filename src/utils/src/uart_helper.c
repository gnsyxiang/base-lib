/**
 *
 * Release under GPLv2.
 * 
 * @file    uart_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/12 2017 22:47
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/12 2017      create the file
 * 
 *     last modified: 12/12 2017 22:47
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#include "log_helper.h"
#include "file_helper.h"

#define UART_HELPER_GB
#include "uart_helper.h"
#undef UART_HELPER_GB

int uart_open(const char *path)
{
	int fd;

	if (!path)
		return -1;

    fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd < 0) {
        perror("open serial porterror!\n");
		return -1;
    }

	if (fcntl(fd, F_GETFL, 0) < 0) {
		log_i("fcntl faild");
		return -1;
	}

	return fd;
}

void uart_close(int fd)
{
	close(fd);
}

int uart_init(int fd, int baude, int c_flow, int bits, char parity, int stop)
{
	struct termios options;

	/*获取终端属性*/
	if(tcgetattr(fd, &options) < 0) {
		perror("tcgetattr error");
		return -1;
	}


	/*设置输入输出波特率，两者保持一致*/
	switch(baude) {
		case 4800:
			cfsetispeed(&options,B4800);
			cfsetospeed(&options,B4800);
			break;
		case 9600:
			cfsetispeed(&options,B9600);
			cfsetospeed(&options,B9600);
			break;
		case 19200:
			cfsetispeed(&options,B19200);
			cfsetospeed(&options,B19200);
			break;
		case 38400:
			cfsetispeed(&options,B38400);
			cfsetospeed(&options,B38400);
			break;
		default:
			fprintf(stderr,"Unkown baude!\n");
			return -1;
	}

	/*设置控制模式*/
	options.c_cflag |= CLOCAL;//保证程序不占用串口
	options.c_cflag |= CREAD;//保证程序可以从串口中读取数据

	/*设置数据流控制*/
	switch(c_flow) {
		case 0://不进行流控制
			options.c_cflag &= ~CRTSCTS;
			break;
		case 1://进行硬件流控制
			options.c_cflag |= CRTSCTS;
			break;
		case 2://进行软件流控制
			options.c_cflag |= IXON|IXOFF|IXANY;
			break;
		default:
			fprintf(stderr,"Unkown c_flow!\n");
			return -1;
	}

	/*设置数据位*/
	switch(bits) {
		case 5:
			options.c_cflag &= ~CSIZE;//屏蔽其它标志位
			options.c_cflag |= CS5;
			break;
		case 6:
			options.c_cflag &= ~CSIZE;//屏蔽其它标志位
			options.c_cflag |= CS6;
			break;
		case 7:
			options.c_cflag &= ~CSIZE;//屏蔽其它标志位
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag &= ~CSIZE;//屏蔽其它标志位
			options.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr,"Unkown bits!\n");
			return -1;
	}

	/*设置校验位*/
	switch(parity) {
		/*无奇偶校验位*/
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;//PARENB：产生奇偶位，执行奇偶校验
			break;
			/*设为空格,即停止位为2位*/
		case 's':
		case 'S':
			options.c_cflag &= ~PARENB;//PARENB：产生奇偶位，执行奇偶校验
			options.c_cflag &= ~CSTOPB;//CSTOPB：使用两位停止位
			break;
			/*设置奇校验*/
		case 'o':
		case 'O':
			options.c_cflag |= PARENB;//PARENB：产生奇偶位，执行奇偶校验
			options.c_cflag |= PARODD;//PARODD：若设置则为奇校验,否则为偶校验
			options.c_cflag |= INPCK;//INPCK：使奇偶校验起作用
			options.c_cflag |= ISTRIP;//ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
			break;
			/*设置偶校验*/
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;//PARENB：产生奇偶位，执行奇偶校验
			options.c_cflag &= ~PARODD;//PARODD：若设置则为奇校验,否则为偶校验
			options.c_cflag |= INPCK;//INPCK：使奇偶校验起作用
			options.c_cflag |= ISTRIP;//ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
			break;
		default:
			fprintf(stderr,"Unkown parity!\n");
			return -1;
	}

	/*设置停止位*/
	switch(stop) {
		case 1:
			options.c_cflag &= ~CSTOPB;//CSTOPB：使用两位停止位
			break;
		case 2:
			options.c_cflag |= CSTOPB;//CSTOPB：使用两位停止位
			break;
		default:
			fprintf(stderr,"Unkown stop!\n");
			return -1;
	}

	/*设置输出模式为原始输出*/
	options.c_oflag &= ~OPOST;//OPOST：若设置则按定义的输出处理，否则所有c_oflag失效

	/*设置本地模式为原始模式*/
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	/*
	 *ICANON：允许规范模式进行输入处理
	 *ECHO：允许输入字符的本地回显
	 *ECHOE：在接收EPASE时执行Backspace,Space,Backspace组合
	 *ISIG：允许信号
	 */

	/*设置等待时间和最小接受字符*/
	options.c_cc[VTIME] = 0;//可以在select中设置
	options.c_cc[VMIN] = 1;//最少读取一个字符

	/*如果发生数据溢出，只接受数据，但是不进行读操作*/
	tcflush(fd,TCIFLUSH);

	/*激活配置*/
	if(tcsetattr(fd, TCSANOW, &options) < 0) {
		perror("tcsetattr failed");
		return -1;
	}

	return 0;
}

ssize_t uart_read(int fd, char *buf, size_t cnt)
{
	return file_read_timeout(fd, buf, cnt, 3000);
}

ssize_t uart_write(int fd, const char *buf, size_t cnt)
{
	return file_write(fd, buf, cnt);
}


