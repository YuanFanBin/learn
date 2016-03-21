#include "apue.h"

static void f1(void), f2(void);

/* gcc apue.h apue_err.c exercise-8.2.c */
int
main(void)
{
    f1();
    f2();
    _exit(0);
}

static void
f1(void)
{
    pid_t pid;

    if ((pid = vfork()) < 0)
        err_sys("vfork error");
    /* child and parent both return */
}

static void
f2(void)
{
    char buf[1000]; /* automatic variables */
    int  i;

    for (i = 0; i < sizeof(buf); i++)
        buf[i] = 0;
}
/*
 * 说白了就是，子进程把父进程的栈空间给搞得乱七八糟
 * 养了一个熊孩子呗
 */
