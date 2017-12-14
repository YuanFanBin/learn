#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

/* PAINFULLY SLOW VERSION -- example only */
ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char    c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
again:
        if ((rc = read(fd, &c, 1)) == 1) {  /* 每读取一个字节，进行一次系统调用，速度极慢 */
            *ptr++ = c;
            if (c == '\n') {
                break;      /* newline is stored, like fgets() */
            }
        } else if (rc == 0) {
            *ptr = 0;
            return(n - 1);  /* EOF, n - 1 bytes were read */
        } else {
            if (errno == EINTR) {
                goto again;
            }
            return(-1);     /* error, errno set by read */
        }
    }
    *ptr = 0;   /* null terminate like fgets() */
    return(n);
}
