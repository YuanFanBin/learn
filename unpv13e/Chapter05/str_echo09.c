#include <unistd.h>     /* ssize_t, read, write */
#include <errno.h>      /* errno */
#include "error.h"
#include "sum.h"

#include "readline.c"

#define MAXLINE 4096    /* max text line length */

/* 客户端与服务端之间传递二进制 */
void str_echo(int sockfd)
{
    ssize_t         n;
    struct args     args;
    struct result   result;

    for ( ; ; ) {
        if ((n = read(sockfd, &args, sizeof(args))) == 0) {
            return; /* connection closed by other end */
        }
        result.sum = args.arg1 + args.arg2;
        if (write(sockfd, &result, sizeof(result)) != sizeof(result)) {
            err_sys("write error");
        }
    }
}
