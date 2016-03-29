#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
abort(void) /* POSIX-style abort() function */
{
    sigset_t         mask;
    struct sigaction action;

    /* Caller can't ignore SIGABRT, if so reset to default */
    sigaction(SIGABRT, NULL, &action);
    if (action.sa_handler == SIG_IGN) {
        action.sa_handler = SIG_DFL;
        sigaction(SIGABRT, &action, NULL);
    }
    if (action.sa_handler == SIG_DFL)
        fflush(NULL);  /* flush all open stdio streams */

    /* Caller can't break SIGABRT; make sure it's unblocked */
    sigfillset(&mask);
    sigdelset(&mask, SIGABRT); /* mask has only SIGABRT turned off */
    sigprocmask(SIG_SETMASK, &mask, NULL);
    /* kill返回前该信号就被传送给了该进程 */
    kill(getpid(), SIGABRT); /* send the signal */

    /* If we're here, process caught SIGABRT and returned */
    /* 在信号处理handler中可能又产生了输出，我们需要再次冲洗缓冲区 */
    fflush(NULL); /* flush all open stdio streams */
    action.sa_handler = SIG_DFL;
    sigaction(SIGABRT, &action, NULL); /* reset to default */
    sigprocmask(SIG_SETMASK, &mask, NULL); /* just in case ... */
    kill(getpid(), SIGABRT); /* and one more time */
    exit(1);
}
