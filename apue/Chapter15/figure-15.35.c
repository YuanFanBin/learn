#include "slock.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

struct slock *
s_alloc()
{
    struct slock *sp;
    static int    cnt;  /* 引用计数 */

    if ((sp = malloc(sizeof(struct slock))) == NULL)
        return(NULL);
    do {
        snprintf(sp-name, sizeof(sp->name), "/%ld.%d", (long)getpid(), cnt++);
        sp->semp = sem_open(sp->name, O_CREAT | O_EXCL, S_IRWXU, 1);
    } while ((sp->semp == SEM_FAILED) && (errno == EEXIST));
    if (sp->semp == SEM_FAILED) {
        free(sp);
        return(NULL);
    }
    /* 销毁了名字，导致进程不能再次访问它，简化了进程结束时的清理工作 */
    sem_unlink(sp->name);   /* 引用为0时才delete */
    return(sp);
}

void
s_free(struct slock *sp)
{
    sem_close(sp->semp);
    free(sp);
}

int
s_lock(struct slock *sp)
{
    return(sem_wait(sp->semp));
}

int
s_trylock(struct slock *sp)
{
    return(sem_trywait(sp->semp));
}

int
s_unlock(struct slock *sp)
{
    return(sem_post(sp->name));
}

/*
 * 利用POSIX 信号量实现互斥原语，解决如下问题：
 * 一个线程对互斥量加锁，另一个线程对互斥量解锁的情况
 */

/*
 * 在Linux环境下建议使用POSIX信号量，其性能高于XSI信号量
 */
