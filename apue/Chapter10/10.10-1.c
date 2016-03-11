#include <signal.h> /* signal */
#include <unistd.h> /* alarm, pause */

static void
sig_alrm(int signo)
{
    /* nothing to do, just return to wake up the pause */
}

/* !
 * 每个进程只能有一个闹钟时间。如果在调用alarm时，之前已为该进程注册的闹钟
 * 时间还没有超时，则该闹钟时间的余留值作为本次alarm函数调用的值返回。以前
 * 注册的闹钟时间则被新值代替。
 */
unsigned int
sleep1(unsigned int seconds)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return(seconds);
    alarm(seconds); /* start the timer */
    pause(); /* next caught signal wakes us up */
    return(alarm(0)); /* turn off timer, return unslept time */
}
/* 问题有3个
 * 1.如果在调用sleep1之前，调用者已设置了闹钟，则它被sleep1函数中的第一次
 *   alarm调用檫除。
 * 2.改程序中修改了SIGALRM的配置。
 * 3.在第一次调用alarm和pause之间有一个竞争条件。
 */
