#include "apue.h"
#include <sys/types.h> /* getpwnam */
#include <pwd.h> /* getpwnam */
#include <signal.h> /* signal */
#include <unistd.h> /* alarm */

static void
my_alarm(int signo)
{
    struct passwd *rootptr;

    printf("in signal handler\n");
    if ((rootptr = getpwnam("root")) == NULL)
        err_sys("getpwnam(root) error");
    alarm(1);
}

/* gcc 10.6.c apue.h apue_err.c */
int
main(void)
{
    struct passwd *ptr;

    signal(SIGALRM, my_alarm);
    alarm(1);
    for ( ; ; ) {
        if ((ptr = getpwnam("fanbin")) == NULL)
            err_sys("getpwnam error");
        if (strcmp(ptr->pw_name, "fanbin") != 0)
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
    }
}
/*
 * 保证函数的可重入性的方法：
 * 1）在写函数时候尽量使用局部变量（例如寄存器、堆栈中的变量）；
 * 2）对于要使用的全局变量要加以保护（如采取关中断、信号量等互斥方法），
 * 这样构成的函数就一定是一个可重入的函数。
 *
 * 满足下列条件的函数多数是不可重入（不安全）的：
 * 1）函数体内使用了静态的数据结构；
 * 2）函数体内调用了malloc() 或者 free() 函数；
 * 3）函数体内调用了标准 I/O 函数。
 *
 * 如何将一个不可重入的函数改写成可重入函数呢？把一个不可重入函数变成可重入
 * 的唯一方法是用可重入规则来重写它。其实很简单，只要遵守了几条很容易理解的
 * 规则，那么写出来的函数就是可重入的：
 * 1）不要使用全局变量。因为别的代码很可能改变这些变量值。
 * 2）在和硬件发生交互的时候，切记执行类似 disinterrupt() 之类的操作，就是关闭
 * 硬件中断。完成交互记得打开中断，在有些系列上，这叫做“进入/ 退出核心”。
 * 3）不能调用其它任何不可重入的函数。
 * 4）谨慎使用堆栈。
 */
/* 可重入函数，简单理解为安全函数，详细说明参考以下博客内容
 * http://blog.csdn.net/tennysonsky/article/details/45127125 
 */
