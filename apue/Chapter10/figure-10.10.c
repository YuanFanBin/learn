#include "apue.h"

static void sig_alrm(int);

/* gcc apue.h apue_err.c figure-10.10.c */
int
main(void)
{
    int  n;
    char line[MAXLINE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");

    alarm(10);
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");
    alarm(0);

    write(STDOUT_FILENO, line, n);
    exit(0);
}

static void
sig_alrm(int signo)
{
    /* nothing to do, just return to interrupt the read */
}

/* alarm处理用于sleep函数外，还常用于对可能阻塞的操作设置时间上限值 */
/* 问题：
 * 1.与之前相同：第一次alarm调用和read调用之间存在竞争。
 * 2.若系统调用是自动重启动的(APUE-3 10.5)，则当从SIGALRM信号处理程序返回时，read
 *   并不被中断，这种情况下，设置时间限制不起作用。
 */
