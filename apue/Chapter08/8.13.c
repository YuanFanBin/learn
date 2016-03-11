#include "apue.h"
#include <sys/wait.h>
#include <errno.h> /* errno, EINTR */
#include <unistd.h> /* fork, execl, waitpid, _exit */

/* 系统函数system，对系统的依赖性很强 
 * 如果cmdstring是一个空指针，则仅当命令处理程序可用时，system返回非
 * 0值，这一特性可以确定在一个给定的操作系统上是否支持system函数。在
 * UNIX中总是可用的。
 * */

int system (const char *cmdstring);
void pr_exit(int status);

/* gcc 8.13-1.c apue.h apue_err.c */
int
main(void)
{
    int status;

    if ((status = system("date")) < 0) {
        err_sys("system() error");
    }
    pr_exit(status);

    if ((status = system("nosuchcommand")) < 0) {
        err_sys("system() error");
    }
    pr_exit(status);

    if ((status = system("who; exit 44")) < 0) {
        err_sys("system() error");
    }
    pr_exit(status);

    exit(0);
}

int
system(const char *cmdstring) /* version without signal handling */
{
    pid_t pid;
    int   status;

    if (cmdstring == NULL) {
        return (1); /* always a command processor with UNIX */
    }

    if ((pid = fork()) < 0) {
        status = -1; /* probably out of process */
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmdstring, (char*)0);
        _exit(127); /* execl error */  /* system calls */
    } else {
        while(waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1; /* error other than EINTR from waitpid() */
                break;
            }
        }
    }

    return (status);
}

/* 8.6-1.c */
void
pr_exit(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termiantion, signal number = %d%s\n\n",
                WTERMSIG(status),
#ifdef WCOREDUMP
                WCOREDUMP(status) ? " (core file generated)" : "");
#else
                "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stopped, signal number = %d\n\n", WSTOPSIG(status));
    }
}
