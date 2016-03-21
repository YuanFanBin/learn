#include "apue.h"
#include <unistd.h> /* vfork, getpid */
#include <sys/types.h> /* vfork */
#include <stdlib.h> /* exit */

int globvar = 6; /* external variable in initialized data */

/* gcc apue.h apue_err.c figure-8.3.c */
int
main(void)
{
    int   var; /* automatic variable on the stack */
    pid_t pid;
    /* exercise 8.1 */
    int   i;
    char buf[512];

    var = 88;
    printf("before vfork\n"); /* we don't flush stdio */
    if ((pid = vfork()) < 0) { /* vfork保证子进程先执行 */
        err_sys("vfork error");
    } else if (pid == 0) { /* child */
        printf("child pid = %ld\n", (long)getpid());
        globvar++; /* modify parent's variables */
        var++;
        //fclose(stdout); /* exercise 8.1 */
        _exit(0);
    }
    sleep(5);
    /* parent continues here */
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);

    /* exercise 8.1 */
    /*
    i = printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    sprintf(buf, "%d\n", i);
    write(STDOUT_FILENO, buf, strlen(buf));
    */

    exit(0);
}
/* 孤儿进程：一个父进程退出，而它的一个或多个子进程还在运行，那么那些子进程
 * 将成为孤儿进程。孤儿进程将被init进程(进程号为1)所收养，并由init进程对它们
 * 完成状态收集工作。
 *
 * 僵尸进程：一个进程使用fork创建子进程，如果子进程退出，而父进程并没有调用
 * wait或waitpid获取子进程的状态信息，那么子进程的进程描述符仍然保存在系统中。
 * 这种进程称之为僵死进程。
 * 以上两点均可在 wikipedia 获取更详细的解释。
 */
/* 子进程并不将父进程的地址空间完全复制到子进程中，因为子进程会立即调用
 * exec(或exit), 于是也就不会引用该地址空间。 子进程中修改会直接更改父进程
 * 数据，若在子进程中return则会造成错误，因共用了父进程堆栈。
 */
/* 参考：http://coolshell.cn/articles/12103.html */
/* 参考：http://www.cnblogs.com/Anker/p/3271773.html */
/* 参考：http://www.cnblogs.com/Anker/archive/2012/12/16/2820526.html */

/* exercise: 
 * 这里假设子进程调用exit时关闭I/O流，但不关闭文件描述符STDOUT_FILENO。
 * 有些版本的标准I/O库会关闭与标准输出相关联的文件描述符从而引起write
 * 标准输出失败。在这种情况下，调用dup将标准输出复制到另一个描述符，
 * write则使用新复制的文件描述符。(都是共享数据的锅)
 * PS:
 * 看某些优秀源码时能够看到这种现象。
 * memcached源码中经常使用dup（原因没深究）
 */
