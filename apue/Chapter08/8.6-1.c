#include "apue.h"
#include <sys/wait.h> /* 宏 */
#include <stdlib.h> /* wait, abort */
#include <unistd.h> /* fork */

void
pr_exit(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termiantion, signal number = %d%s\n",
                WTERMSIG(status),
#ifdef WCOREDUMP
                WCOREDUMP(status) ? " (core file generated)" : "");
#else
                "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
    }
}

/* gcc 8.6-1.c apue.h apue_err.c */
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
