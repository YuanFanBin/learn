#include "apue.h"
#include <sys/wait.h>
#include <unistd.h> /* execle, execlp, setenv */

char *env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

/* gcc apue.h apue_err.c figure-8.16.c */
/* 依赖 figure-8.17.c -o echoall */
int
main(void)
{
    int           i;
    pid_t         pid;
    extern char **environ;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* specify pathname, specify environment */
        /* 更改了echoall进程的环境变量 */
        if (execle("/home/fanbin/yuanfanbin/learn/apue/echoall", "echoall", "myarg1",
                    "My ARG2", (char*)0, env_init) < 0) {
            err_sys("execle error");
        }
    }

    if (waitpid(pid, NULL, 0) < 0) {
        err_sys("wait error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* specify filename, inherit environment */
        printf("\n\nchild pid = %ld\n", (long)getpid());
        /* 更改了子进程&孙子进程中的环境变量 */
        setenv("PATH", "/home/fanbin/yuanfanbin/learn/apue/", 1); // 重写PATH
        if (execlp("echoall", "echoall", "only 1 arg", (char*)0) < 0) {
            err_sys("execlp error");
        }
    }

    sleep(2);
    /* 父进程的环境变量是没有改变的 */
    printf("\n\nThis is parent environ\n");
    printf("parent, PATH = %s\n", getenv("PATH"));
    for (i = 0; environ[i] != NULL; i++) { /* echo all env strings */
        //printf("environ[%d]: %s\n", i, environ[i]);
    }

    exit(0);
}
