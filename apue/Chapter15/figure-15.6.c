#include "apue.h"
#include <sys/wait.h>

#define DEF_PAGER   "/bin/more"     /* default pager program */

/* gcc apue.h apue_err.c figure-15.6.c */
int
main(int argc, char *argv[])
{
    int     n;
    int     fd[2];
    pid_t   pid;
    char   *pager, *argv0;
    char    line[MAXLINE];
    FILE   *fp;

    if (argc != 2)
        err_quit("usage: a.out <pathname>");

    if ((fp = fopen(argv[1], "r")) == NULL)
        err_sys("can't open %s", argv[1]);
    if (pipe(fd) < 0)
        err_sys("pipe error");

    if ((pid = fork()) < 0) {
        err_sys("pipe error");
    } else if (pid > 0) {   /* parent */
        close(fd[0]);       /* close read end */

        /* parent copies argv[1] to pipe */
        while (fgets(line, MAXLINE, fp) != NULL) {
            n = strlen(line);
            if (write(fd[1], line, n) != n)
                err_sys("write error to pipe");
        }
        if (ferror(fp))
            err_sys("fgets error");

        close(fd[1]);   /* close writed end of pipe for reader */

        if (waitpid(pid, NULL, 0) < 0) /* 阻塞等待子进程结束，避免僵尸进程 */
            err_sys("waitpid error");
        exit(0);
    } else {    /* child */
        close(fd[1]);   /* close write end */
        if (fd[0] != STDIN_FILENO) { /* 作为一个保护措施，将两个描述符进行比较 */
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
                err_sys("dup2 error to stdin");
            close(fd[0]);   /* don't need this after dup2 */
        }
        /* get arguments for execl() */
        if ((pager = getenv("PAGER")) == NULL)
            pager = DEF_PAGER;
        /* pager == /bin/more */
        if ((argv0 = strrchr(pager, '/')) != NULL) /* 一直搞不懂这个干嘛用 */
            argv0++;    /* step past rightmost slash, 定位到/more */ 
        else
            argv0 = pager;  /* no slash in pager */
        /* argv0 = more */

        if (execl(pager, argv0, (char *)0) < 0)
            err_sys("execl error for %s", pager);
    }
    exit(0);
}
