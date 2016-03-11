#include "apue.h"
#include <unistd.h> /* STDOUT_FILENO, fork, sleep */

int  globvar = 6; /* external variable in initialized data */
char buf[] = "a write to stdout\n";

/* 一般来说，在fork之后是父进程先执行还是子进程先执行是不确定的。 
 * 这取决于内核所使用的调度算法。如果要求父进程和子进程直线相互同步，
 * 则要求某种形式的进程间通信（10.16） */
/* COW技术 */
/* gcc 8.3.c apue.h apue_err.c */
/* $ ./a.out         # 行缓冲，数据已输出
 * $ ./a.out > temp  # 全缓冲，数据未输出，缓冲数据被copy
 */
int
main(void)
{
    int   var; /* automatic variable on the stack */
    pid_t pid;

    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) -1 ) {
        err_sys("write error");
    }
    printf("before fork\n");  /* we don't flush stdout */

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        printf("child pid = %d\n", getpid());
        globvar++; /* modify variables */
        var++;
    } else {
        sleep(2); /* parent */
    }

    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);

    exit(0);
}

/* 本例中，当父进程等待子进程时，子进程写到标准输出；而在子进程终止后，父进程
 * 也写到标准输出上，并且知道其输出会追加在子进程所写数据之后。如果父进程和子
 * 进程不共享同一个文件偏移量，要实现这种形式的交互就要困难得多，可能需要父进
 * 程显示地动作。
 */
/* 某些操作系统将fork之后执行exec组合成一个操作，称为swapn */
/* 操作系统将这两个操作分开，使得子进程在fork和exec之间可以更改自己的属性，
 * 如I/O重定向、用户ID、信号安排等。 */
/* fork， 多线程存在的问题
 * 参考资料：http://www.cnblogs.com/liyuan989/p/4279210.html
 */
