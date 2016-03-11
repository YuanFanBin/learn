#include "apue.h"
#include <pthread.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void
prepare(void)
{
    int err;

    printf("preparing locks...\n");
    if ((err = pthread_mutex_lock(&lock1)) != 0)
        err_cont(err, "can't lock lock1 in prepare handler");
    if ((err = pthread_mutex_lock(&lock2)) != 0)
        err_cont(err, "can't lock lock2 in prepare handler");
}

void
parent(void)
{
    int err;
    
    printf("parent unlocking locks...\n");
    if ((err = pthread_mutex_unlock(&lock1)) != 0)
        err_cont(err, "can't unlock lock1 in parent handler");
    if ((err = pthread_mutex_unlock(&lock2)) != 0)
        err_cont(err, "can't unlock lock2 in parent handler");
}

void
child(void)
{
    int err;

    printf("child unlocking locks...\n");
    if ((err = pthread_mutex_unlock(&lock1)) != 0)
        err_cont(err, "can't unlock lock1 in child handler");
    if ((err = pthread_mutex_unlock(&lock2)) != 0)
        err_cont(err, "can't unlock lock2 in child handler");
}

void *
thr_fn(void *arg)
{
    printf("thread started...\n");
    pause();
    return(0);
}

/* 有关线程和fork还需查询资料了解 */
/* gcc 12.9.c apue.h apue_err.c -lpthread */
int
main(void)
{
    int         err;
    pid_t       pid;
    pthread_t   tid;

    if ((err = pthread_atfork(prepare, parent, child)) != 0)
        err_exit(err, "can't install fork handlers");
    if ((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0)
        err_exit(err, "can't create thread");

    sleep(2);
    printf("parent about to fork...\n");

    if ((pid = fork()) < 0)
        err_quit("fork failed");
    else if (pid == 0) /* child */
        printf("child returned from fork\n");
    else /* parent */
        printf("parent returned from fork\n");
    exit(0);
}

/* 因为prepare fork处理程序获取了所有的锁，父进程中的内存和子进程中的
 * 内存中的内存内容在开始的时候是相同的。
 * 产生效果：
 *  - 在父进程中加过的锁，在子进程中也加锁了（内存副本）;
 *  - 父进程解锁与子进程解锁不冲突，进程写时复制，操作不同的内存区域。
 */


/* 12.10 线程和IO
 * 可以使用pread，pwrite来解决并发线程对同一文件写操作的问题。
 * (Nginx源码有实例)
 */

/* 练习12.1：
 *  终端：行缓冲；文件：全缓冲
 *  写文件，printf默认全缓冲，缓冲区满或进程关闭流时才写入文件，
 *  fork后内存数据被复制，写文件时父子进程分别将块内数据写入文件。
 */
