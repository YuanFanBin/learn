#include "apue.h"
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

static jmp_buf env_alrm;

unsigned int sleep2(unsigned int);
static void sig_int(int);

/* gcc 10.10-2.c apue.h apue_err.c */
/* $ ./a.out
 * ^C       # 等大约4秒后
 * sig_int starting
 * sleep2 returned: 0
 */
int
main(void)
{
    unsigned int unslept;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    unslept = sleep2(5);
    printf("sleep2 returned: %u\n", unslept);
    exit(0);
}
/* 执行现象，SIGALRM的中断处理程序使另一个信号处理程序sig_int提早结束，
 * 即时它未完成。这是个问题
 */

static void sig_int(int signo)
{
    int          i, j;
    volatile int k; /* 7.10-2.c */

    /*
     * Turn these loops to run for more than 5 seconds
     * on whatever system this test program is run.
     */
    printf("\nsig_int starting\n");
    for (i = 0; i < 300000; i++)
        for (j = 0; j < 4000; j++)
            k += i * j;
    printf("sig_int finished\n");
    /* 这里面捕获到了中断信号SIGALRM，处理对应中断程序，环境恢复失败 */
}


static void
sig_alrm(int signo)
{
    longjmp(env_alrm, 1); /* 解决第三个问题:竞争条件 */
}

unsigned int
sleep2(unsigned int seconds)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return(seconds);
    if (setjmp(env_alrm) == 0) {
        alarm(seconds);  /* start the timer */
        pause();         /* next caught signal wakes us up */
    }
    return(alarm(0));    /* turn off timer, return unslept time */
}
