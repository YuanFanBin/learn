#include "apue.h"
#include <sys/wait.h>

/* gcc apue.h apue_err.c figure-15.15.c */
int
main(void)
{
    char    line[MAXLINE];
    FILE   *fpin;

    if ((fpin = popen("./myuclc", "r")) == NULL)    /* 从过滤程序中获取输入 */
        err_sys("popen error");
    for ( ; ; ) {
        fputs("prompt> ", stdout);
        fflush(stdout);
        if (fgets(line, MAXLINE, fpin) == NULL)     /* read from pipe */
            break;
        if (fputs(line, stdout) == EOF)
            err_sys("fputs error to pipe");
    }
    if (pclose(fpin) == -1)
        err_sys("pclose error");
    putchar('\n');
    exit(0);
}
