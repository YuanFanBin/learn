#include "apue.h"
#include <stdio.h> /* fgetc, fputc, stdin, stdout, ferror */
#include <stdlib.h> /* exit */

/* gcc apue.h apue_err.c figure-5.5.c */
int
main(int argc, char *argv[])
{
    char buf[MAXLINE];

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (fputs(buf, stdout) == EOF) {
            err_sys("output error");
        }
    }
    if (ferror(stdin)) {
        err_sys("input error");
    }

    exit(0); /* exit函数会冲洗任何未写的数据，然后关闭所有打开的流 */
}
