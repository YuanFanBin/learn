#include "apue.h"
#include <sys/wait.h>

/* gcc 8.12.c apue.h apue_err.c */
/* 依赖8.10-2.c -o echoall */
int
main(void)
{
    pid_t  pid;
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (execl("/home/fanbin/learn/apue/testinterp",
                    "testinterp", "myarg1", "My ARG2", (char*)0) < 0) {
            err_sys("execl error");
        }
    }
    if (waitpid(pid, NULL, 0) < 0) { /* parent */
        err_sys("waitpid error");
    }
    exit(0);
}

/*
 * 解析器格式：
 * #! pathname[optional-argument]
 * !和pathname之间的空格可选
 *
 * chmod + x testinterp
 * #!/home/fanbin/learn/apue/echoall foo
 */
