#include "apue.h"
#include <unistd.h> /* fork, sleep, execl, getpid */
#include <stdlib.h> /* exit, abort */
#include <sys/types.h> /* kill, getpid */
#include <signal.h>  /* kill, SIGKILL */

/* gcc 8.14-1.c apue.h apue_err.c */
/* 为了得到某些会计数据以便查看 */
/* <sys/acct.h> */
int
main(void)
{
    pid_t  pid;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) { /* parent */
        sleep(2);
        exit(2);
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid !=0 ) { /* first child */
        sleep(4);
        abort(); /* terminate with core dump */
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) { /* second child */
        execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", NULL);
        exit(7); /* shouldn't get here */
    }


    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) { /* third child */
        sleep(8);
        exit(0); /* normal exit */
    }

    sleep(6); /* forth child */
    kill(getpid(), SIGKILL); /* terminate w/signal, no core dump */
    exit(6); /* shouldn't get here */
}
