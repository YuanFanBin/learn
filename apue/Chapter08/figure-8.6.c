#include "apue.h"
#include <sys/wait.h>

/* gcc apue.h apue_err.c figure-8.5.c figure-8.6.c */
/* 在<signal.h>中查看各个信号的值 */
int
main(void)
{
    pid_t  pid;
    int    status;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        exit(7);
    }
    if (wait(&status) != pid) { /* wait for child */
        err_sys("wait error");
    }
    pr_exit(status); /* and print its status */


    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        abort(); /* generates SIGABRT */
    }
    if (wait(&status) != pid) { /* wait for child */
        err_sys("wait error");
    }
    pr_exit(status); /* and print its status */


    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        status /= 0;  /* divide by o generates SIGFPE */
    }
    if (wait(&status) != pid) { /* wait for child */
        err_sys("wait error");
    }
    pr_exit(status); /* and print its status */


    exit(0);
}
