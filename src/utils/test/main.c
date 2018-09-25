#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log_helper.h"

int main(int argc, char const* argv[])
{
    log_init("res/log.log", LOG_VERBOSE, 1);

    int cnt = 0;
    while (1) {
        if (cnt++ > 15)
            break;

        log_i("cnt: %d", cnt);
        log_e("cnt: %d", cnt);
    }

    int fd = open("haha.log", O_RDONLY);
    if (fd < 0)
        log_e("open faild");

    return 0;
}

