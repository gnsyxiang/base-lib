#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>

#define MAX_BUFFER_SIZE512

int fd,flag_close;

int open_serial()
{
    //这里的/dev/pts/1是使用mkptych.py虚拟的两个串口名字之一
    fd = open("/dev/pts/20",O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(fd == -1)
    {
        perror("open serial porterror!\n");
        return -1;
    }

    printf("Open serial portsuccess!");
    return 0;
}

int serial_send(void)
{
    char sbuf[] = {"Hello, thisis a serial port test!\n"};
    int retv;
    struct termios option;

    retv =open_serial();
    if(retv <0)
    {
        perror("open serial porterror!\n");
        return -1;
    }

    printf("Ready for sendingdata...\n");

    tcgetattr(fd,&option);
    cfmakeraw(&option);

    cfsetispeed(&option,B9600);
    cfsetospeed(&option,B9600);

    tcsetattr(fd, TCSANOW,&option);

    int count = 0;
    int length =sizeof(sbuf);
    while (count++ < 10) {
        sleep(1);

        retv = write(fd, sbuf,length);
        if(retv == -1)
        {
            perror("Write dataerror!\n");
            return -1;
        }
    }

    close(fd);

    printf("The number of charsent is %d\n", retv);
    return 0;
}
