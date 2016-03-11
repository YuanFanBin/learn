#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_SERVER "/tmp/fifoserver"

/*
 * 把读程序编译成两个不同版本：
 *     阻塞读版本       : br
 *     以及非阻塞读版本 : nbr
 */
/* gcc 15.5-2.c */
/* sudo ... */
int
main(int argc, char *argv[])
{
    char    r_buf[4096*2];
    int     fd;
    int     r_size;
    int     ret_size;

    r_size = atoi(argv[1]);
    fprintf(stdout, "requred real read bytes %d\n", r_size);
    memset(r_buf, 0, sizeof(r_buf));

    /* 非阻塞读, nbr */
    //fd = open(FIFO_SERVER, O_RDONLY|O_NONBLOCK, 0);
    /* 阻塞读, br */
    fd = open(FIFO_SERVER, O_RDONLY, 0);
    if (fd == -1) {
        fprintf(stdout, "open %s for read error\n", strerror(errno));
        exit(0);
    }
    while (1) {
        memset(r_buf, 0, sizeof(r_buf));
        ret_size = read(fd, r_buf, r_size);
        if (ret_size == -1 && errno == EAGAIN)
            fprintf(stdout, "no data avlaible\n");
        fprintf(stdout, "real read bytes %d\n", ret_size);
        sleep(1);
    }
    pause();
    unlink(FIFO_SERVER);
}
