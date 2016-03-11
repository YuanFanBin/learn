#include "apue.h"

#ifdef SOLARIS
#define PSCMD   "ps -a -o pid,ppid,s,tty,comm"
#else
#define PSCMD   "ps -o pid,ppid,state,tty,command"
#endif

/* 创建僵死进程 */
/* gcc e8.6.c apue.h apue_err.c */
int
main(void)
{
    pid_t  pid;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) /* child */
        exit(0);

    /* parent */
    sleep(4);
    system(PSCMD);
    exit(0);
}
/* 使用Zombie状态表示进程退出了，正在等待父进程收集信息中。 */
