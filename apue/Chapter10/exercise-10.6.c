#include "apue.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int add(int);

/* gcc apue.h apue_err.c figure-10.24.c exercise-10.6.c */
int main(int argc, char *argv[])
{
    int     fd;
    pid_t   pid;
    int     n = 10; /* 交替运行10次，查看运行结果 */

    if ((fd = open("tmp.file", O_CREAT | O_TRUNC | O_RDWR, "0666")) < 0)
        err_sys("open error");

    if (write(fd, "0", 1) != 1)
        err_sys("write error");

    TELL_WAIT();            /* Figure-10.24.c */
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) {    /* child */
        fprintf(stdout, "child pid = %d\n", getpid());
        while (n--) {
            WAIT_PARENT();          /* 父进程先执行 */
            if (add(fd) < 0)
                err_sys("add error"); /* do somethind */
            TELL_PARENT(getppid()); /* 告诉父进程，子进程执行结束 */
        }
        _exit(0);

    } else {
        fprintf(stdout, "parent pid = %d\n", getpid());
        while (n--) {
            if (add(fd) < 0)
                err_sys("add error"); /* do something */
            TELL_CHILD(pid);   /* 子进程后执行 */
            WAIT_CHILD();      /* 等待子进程执行完成 */
        }
    }
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");
    close(fd);
    exit(0);
}

int add(int fd)
{
    char buf[MAXLINE];
    if (lseek(fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "lseek failed\n");
        return(-1);
    }
    if (read(fd, buf, MAXLINE) < 0) {
        fprintf(stderr, "read failed, fd = %d\n", fd);
        return(-1);
    }

    sprintf(buf, "%d", atoi(buf) + 1);
    fprintf(stdout, "pid = %d, cur = %s\n", getpid(), buf);

    if (lseek(fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "lseek failed\n");
        return(-1);
    }
    if (write(fd, buf, strlen(buf)) < 0) {
        fprintf(stderr, "write failed\n");
        return(-1);
    }
    return(0);
}
