#include "apue.h"
#include <unistd.h> /* pause */
#include <signal.h> /* SIGXXX */

static void sig_usr(int); /* one handler for both signals */

/* gcc apue.h apue_err.c figure-10.2.c */
int
main(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("can't catch SIGUSR1");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("can't catch SIGUSR2");
    for ( ; ; )
        pause();    /* man 7 signal or man pause */
}

static void
sig_usr(int signo) /* argument is signal number */
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGUSR2)
        printf("received SIGUSR2\n");
    else
        err_dump("received signal %d\n", signo);
}
/*
 * $ ./a.out &
 * [1] 1274
 * $ kill -USR1 1274
 * $ kill -USR2 1274
 * $ kill 1274       # SIGTERM
 */
/* exec函数将原先设置为要捕捉的信号都更改为默认动作，其他信号的状态
 * 不变。
 */
/* fork子进程后，子进程继承父进程的信号处理方式 */
