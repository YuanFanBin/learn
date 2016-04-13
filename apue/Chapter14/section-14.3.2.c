/* 15.8 信号量-实例：信号量、记录锁和互斥量的时间比较 */
/*  
 *  figure-8.12.c   --进程同步模板
 *  figure-10.24.c  -- 信号实现
 *  figure-14.3.2.c -- 记录锁实现（自己实现的）
 *  figure-15.7.c   -- 管道实现
 */
/* figure-14.5.c */
#include "apue.h"
#include <fcntl.h>

static char lockfile[] = "/tmp/lockfile";
static int lock_fd = -1;

/* !!! 根本就不对 */
void
TELL_WAIT(void)
{
    if ((lock_fd = creat(lockfile, FILE_MODE)) < 0)
        err_sys("creat error");
    writew_lock(lock_fd, 0, SEEK_SET, 2);
}

void
TELL_PARENT(pid_t pid)
{
    un_lock(lock_fd, 0, SEEK_SET, 1);
}

void
WAIT_PARENT(void)
{
    /* figure-14.5.c apue.h */
    writew_lock(lock_fd, 1, SEEK_SET, 1);
}

void
TELL_CHILD(pid_t pid)
{
    un_lock(lock_fd, 1, SEEK_SET, 1);
}

void
WAIT_CHILD(void)
{
    writew_lock(lock_fd, 0, SEEK_SET, 1);
}

/* 实现错了，应该实现成 wait(P), signal(V)*/
