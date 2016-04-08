#include "apue.h"
#include <ctype.h>
#include <fcntl.h>
#include <aio.h> /* -lrt */
#include <errno.h>

#define BSZ 4096
#define NBUF 1      /* 只需1个即可 */

enum rwop {
    UNUSED = 0,
    READ_PENDING = 1,
    WRITE_PENDING = 2
};

struct buf {
    enum rwop       op;
    int             readn;
    struct aiocb    aiocb;
    unsigned char   data[BSZ];
};

struct buf bufs[NBUF];
unsigned char
translate(unsigned char c)
{
    /* same as before */
    if (isalpha(c)) {
        if (c >= 'n')
            c -= 13;
        else if (c >= 'a')
            c += 13;
        else if (c >= 'N')
            c -= 13;
        else
            c += 13;
    }
    return(c);
}

/* 参考资料：https://www.ibm.com/developerworks/cn/linux/l-async/ */

/*
 *                  blocking                            non-blocking
 * synchronous   |   read/write                     |    read/write(O_NONBLOCK)
 * asynchronous  |  I/O multiplexing(select, poll)  |   AIO
 */

/* 利用异步IO实现figure-14.20.c的读写数据 */
/* gcc apue.h apue_err.c exercise-14.8.c -lrt -o rot13 */
int
main(int argc, char *argv[])
{
    int                 ifd, ofd, i, j, n, err, numr, bnum;
    const struct aiocb *aiolist[NBUF];
    off_t               off = 0;

    if (fdopen(STDIN_FILENO, "r") < 0)
        err_sys("can't open %s", argv[1]);
    ifd = STDIN_FILENO;
    if (fdopen(STDOUT_FILENO, "w+") < 0)
        err_sys("can't create %s", argv[2]);
    ofd = STDOUT_FILENO;

    /* initialize the buffers */
    for (i = 0; i < NBUF; i++) {
        bufs[i].op = UNUSED;
        bufs[i].readn = 0;
        bufs[i].aiocb.aio_buf = bufs[i].data;
        bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
        aiolist[i] = NULL;
    }

    numr = 0, bnum = 0;
    for (;;) {
        for (i = 0; i < NBUF; i++) {
            switch(bufs[i].op) {
            case UNUSED:
                /*
                 * Read from the input file if more data
                 * remains unread.
                 */
                bufs[i].op = READ_PENDING;
                bufs[i].aiocb.aio_fildes = ifd;
                bufs[i].aiocb.aio_offset = off;
                off += BSZ;
                bufs[i].aiocb.aio_nbytes = BSZ;
                if (aio_read(&bufs[i].aiocb) < 0)
                    err_sys("aio_read failed");
                aiolist[i] = &bufs[i].aiocb;
                numr++;
                break;

            case READ_PENDING:
                if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
                    continue;
                if (err != 0) {
                    if (err == -1)
                        err_sys("aio_error failed");
                    else
                        err_exit(err, "read failed");
                }

                /*
                 * A read is complete; translate the buffer
                 * and write it.
                 */
                /* 获取实际读取的字节数 */
                if ((n = aio_return(&bufs[i].aiocb)) < 0)
                    err_sys("aio_return failed");
                if (n == 0) {   /* 读结束 */
                    numr--;
                    break;
                }
                bufs[i].readn = n;

                for (j = 0; j < n; j++)
                    bufs[i].data[j] = translate(bufs[i].data[j]);
                bufs[i].op = WRITE_PENDING;
                bufs[i].aiocb.aio_fildes = ofd;
                bufs[i].aiocb.aio_nbytes = n;
                if (aio_write(&bufs[i].aiocb) < 0)
                    err_sys("aio_write failed");
                /* retain our spot in aiolist */
                break;

            case WRITE_PENDING:
                bnum = 1;
                if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
                    continue;
                if (err != 0) {
                    if (err == -1)
                        err_sys("aio_error failed");
                    else
                        err_exit(err, "write failed");
                }

                /*
                 * A write is complete; mark the buffer as unused.
                 */
                if ((n = aio_return(&bufs[i].aiocb)) < 0)
                    err_sys("aio_return failed");
                if (n != bufs[i].aiocb.aio_nbytes)
                    err_quit("short write (%d/%d)", n, BSZ);
                aiolist[i] = NULL;
                bufs[i].op = UNUSED;
                bnum = 0;
                break;
            }
        }
        /* 没有任何读/写操作，退出 */
        if (numr == 0 && bnum == 0) {
            break;
        } else if (numr == 0) {
            if (aio_suspend(aiolist, NBUF, NULL) < 0)
                err_sys("aio_suspend failed");
        }
    }
    bufs[0].aiocb.aio_fildes = ofd;
    if (aio_fsync(O_SYNC, &bufs[0].aiocb) < 0)
        err_sys("aio_fsync failed");
    exit(0);
}
