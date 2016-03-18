#include "apue.h"
#include <stdlib.h> /* atexit */

static void my_exit1(void);
static void my_exit2(void);

/* gcc apue.h apue_err.h figure-7.3.c */
/* atexit用于登记退出函数事件，一个进程可以登记多至32个函数 */
/* 调用函数与登记函数顺序相反，可重复登记，重复调用 */
/* C程序如何启动，APUE 7.3 图 */
/* $ echo $? 查看本进程调用结果，结果由 return/exit指定 */
int 
main(void)
{
    if (atexit(my_exit2) != 0)
        err_sys("can't register my_exit2");
    if (atexit(my_exit1) != 0)
        err_sys("can't register my_exit1");
    if (atexit(my_exit1) != 0)
        err_sys("can't register my_exit1");
    printf("main is done\n");

    return(0);
}

static void
my_exit1(void)
{
    printf("first exit handler\n");
}

static void
my_exit2(void)
{
    printf("second exit handler\n");
}
