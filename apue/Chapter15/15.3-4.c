#include "apue.h"
#include <sys/wait.h>

/* gcc 15.3-4.c apue.h apue_err.c */
int
main(void)
{
    char    line[MAXLINE];
    FILE   *fpin;

    if ((fpin = popen("./myuclc", "r")) == NULL)
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

/* 总结：参考资料
 * 管道&命名管道（FIFO）
 * https://www.ibm.com/developerworks/cn/linux/l-ipc/part1/
 * 命名管道：
 *     当要写入的数据量不大于PIPE_BUF时，linux将保证写入的原子性。
 */
