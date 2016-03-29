#include <sys/wait.h>
#include <errno.h>  /* errno, EINTR */
#include <unistd.h> /* fork, execl, waitpid, _exit */

/* 系统函数system，对系统的依赖性很强 
 * 如果cmdstring是一个空指针，则仅当命令处理程序可用时，system返回非
 * 0值，这一特性可以确定在一个给定的操作系统上是否支持system函数。在
 * UNIX中总是可用的。
 * */

/* Figure-10.28.c 有更详细的实现（处理信号） */

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
