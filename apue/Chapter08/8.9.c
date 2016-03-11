#include "apue.h"
#include <stdio.h> /* setbuf */

static void charatatime(char *);

/* gcc 8.9.c apue.h apue_err.c */
int
main(void)
{
    pid_t  pid;

    //TELL_WAIT(); /* + */
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        //WAIT_PARENT(); /* + */
        charatatime("output from child\n");
        //TELL_PARENT(getppid()); /* + */
    } else {
        //WAIT_CHILD(); /* + */
        charatatime("output from parent\n");
        //TELL_CHILD(pid); /* + */
    }
    exit(0);
}

static void
charatatime(char *str)
{
    char *ptr;
    int   c;

    /* 将stdout设为非缓冲模式，每个字符均调用write系统函数 */
    /* 目的：尽可能的多次在两个进程间切换，以便演示竞争条件 */
    setbuf(stdout, NULL);  /* set unbuffered */
    for (ptr = str; (c = *ptr++) != 0;) {
        putc(c, stdout);
    }
}
