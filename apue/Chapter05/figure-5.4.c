#include "apue.h"
#include <stdio.h> /* getc, putc, stdin, stdout, ferror */
#include <stdlib.h> /* exit */

/* gcc apue.h apue_err.c figure-5.4.c */
int
main(int argc, char *argv[])
{
    int c;

    /* getc, putc 被简单的实现为宏 */
    while((c = getc(stdin)) != EOF) {
        if (putc(c, stdout) == EOF) {
            err_sys("output error");
        }
    }

    if (ferror(stdin)) {
        err_sys("input error");
    }
    exit(0); /* exit函数会冲洗任何未写的数据，然后关闭所有打开的流 */
}
