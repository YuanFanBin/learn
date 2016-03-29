#include "apue.h"
#include <signal.h> /* signal, sigemptyset, sigaddset, sigprocmask */

static void sig_int(int);

/* gcc  apue.h apue_err.c figure-10.14.c figure-10.18.c figure-10.22.c */
/* 保护代码临界区，使其不被特定信号中断 */
int
main(void)
{
    sigset_t newmask, oldmask, waitmask;

    pr_mask("program start: "); /* Figure-10.14 */

    if (signal(SIGINT, sig_int) == SIG_ERR) /* Figure-10.18 */
        err_sys("signal(SIGINT) error");
    sigemptyset(&waitmask);
    sigaddset(&waitmask, SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);

    /*
     * Block SIGINT and save current signal mask.
     */
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLCOK error");

    /*
     * Critical region of code.
     */
    pr_mask("in critical region: ");

    /*
     * Pause, allowing all signal except SIGUSR1.
     */
    if (sigsuspend(&waitmask) != -1) /* 挂起waitmask */
        err_sys("sigsuspend error");

    pr_mask("after return from sigsuspend: ");

    /*
     * Reset signal mask which unblocks SIGINT.
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    /*
     * And continue processing ...
     */
    pr_mask("program exit: ");

    exit(0);
}

static void
sig_int(int signo)
{
    pr_mask("\nin sig_int: ");
}
