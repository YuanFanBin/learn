#include "apue.h"
#include <errno.h>
#include <fcntl.h>

char  buf[500000];

/* gcc apue.h apue_err.c figure-3.12.c figure-14.1.c */
int
main(void)
{
    int     ntowrite, nwrite;
    char   *ptr;

    /* 
     * STDIN_FILENO 与 stdin区别，参考资料：
     * http://blog.csdn.net/yeyuangen/article/details/6781999
     * STDIN_FILENO ==> int;        read, write, ...
     * stdin        ==> FILE *;     fread, fwrite, ...
     */
    ntowrite = read(STDIN_FILENO, buf, sizeof(buf)); /* man unistd.h查看 */
    fprintf(stderr, "read %d bytes\n", ntowrite);

    set_fl(STDOUT_FILENO, O_NONBLOCK); /* figure-3.12.c set nonblocking */

    ptr = buf;
    while(ntowrite > 0) { /* 轮询 */
        errno = 0;
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);
        fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

        if (nwrite > 0) {
            ptr += nwrite;
            ntowrite -= nwrite;
        }
    }

    clr_fl(STDOUT_FILENO, O_NONBLOCK); /* clear nonblocking */

    exit(0);
}

/* 非阻塞IO实例，轮询方式 */
