#include "apue.h"
#include <sys/wait.h> /* waitpid */
#include <sys/types.h>  /* waitpid, getppid */
#include <unistd.h> /* getppid */

/* gcc 8.6-2.c apue.h apue_err.c */
int
main(void)
{
    pid_t   pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* first child */
        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid > 0) {
            exit(0); /* parent from second fork == first child */
        }

        /*
         * We're second child; our parent becomes init as soon
         * as our real parent calls exit() in the statement above.
         * Here's where we'd continue executing, knowing that when
         * we're done, init will reap our status.
         */
        /* 
         * 打印进程ID为1的进程，即init进程。
         * 调大参数，使用命令pstree查看更直观
         */
        sleep(2); /* 等待父进程（first child）结束 */
        printf("second child, parent pid = %ld\n", (long)getppid());
        exit(0); 
    }

    if (waitpid(pid, NULL, 0) != pid) { /* wait for first child */
        err_sys("waitpid error");
    }

    /*
     * We're the parent (the original process); we continue executing,
     * knowing that we're not the parent of the second child.
     */
    exit(0);
}

/*
 * 对于waitpid函数中的pid参数的作用解释如下
 * pid == -1  等待任一子进程。此种情况下，与wait等效
 * pid > 0    等待进程ID与pid相等的进程。
 * pid == 1   等待组ID等于调用进程组ID的任一子进程。
 * pid < -1   等待组ID等于pid绝对值的任一子进程。
 */
/*
 * waitpid 可等待一个特定的进程。
 * waitpid 提供了一个wait的非阻塞版本。（还不知道是哪种）
 * waitpid 通过WUNTRACED和WCONTINUED选项支持作业控制。
 */
/* 
 * 如果一个进程fork一个子进程，但不要等待子进程终止，也不希望子进程处于僵死
 * 状态直到父进程终止，实现这一要求的诀窍是调用fork两次。
 */
