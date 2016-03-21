#include "apue.h"
#include <sys/wait.h>

int
main(void)
{
    pid_t   pid;
    int     status;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {  /* child */
        exit(7);
    }
    if (waitpid(pid, &status, 0) < 0) {
        err_sys("waitpid error");
    }
}

/*
 * ?? 如何获取siginfo信息
 */
