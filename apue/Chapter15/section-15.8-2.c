/* 15.8 - 实例：信号量，记录锁和互斥量的时间比较 */
/* 2.记录锁 */
#include "apue.h"

static char lockfile[] = "/tmp/lockfile2";
static int lock_fd = -1;

void
recordlock_init(void)
{
    if ((lock_fd = creat(lockfile, FILE_MODE)) < 0)
        err_sys("creat error");
}

void
recordlock_wait(void)   /* P */
{
    /* figure-14.5.c apue.h */
    writew_lock(lock_fd, 0, SEEK_SET, 1);
}

void
recordlock_signal(void) /* V */
{
    un_lock(lock_fd, 0, SEEK_SET, 1);
}
