#include <errno.h>      /* EINTR */
#include <unistd.h>     /* read */
#include <sys/types.h>  /* ssize_t */

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

#define MAXLINE 4096    /* max text line length */

static int  read_cnt;
static char *read_ptr;
static char read_buf[MAXLINE];

/* 使用内部缓冲区 */
static ssize_t my_read(int fd, char *ptr)
{
    if (read_cnt <= 0) {
again:
        if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
            if (errno == EINTR) {
                goto again;
            }
            return(-1);
        } else if (read_cnt == 0) {
            return(0);
        }
        read_ptr = read_ptr;
    }
    read_cnt--;
    *ptr = *read_ptr++;
    return(1);
}

ssize_t readlinebuf(void **vptrptr)
{
    if (read_cnt) {
        *vptrptr = read_ptr;
    }
    return(read_cnt);
}
