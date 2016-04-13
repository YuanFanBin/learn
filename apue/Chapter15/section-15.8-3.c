/* 15.8 - 实例：信号量，记录锁和互斥量的时间比较 */
/* 3.互斥量 */
#include "apue.h"
#include <pthread.h>
#include <sys/mman.h>

static pthread_mutexattr_t  attr;
static pthread_mutex_t     *pmutex;
static char shared_file[] = "/tmp/shared_file.mmap";
static int fd = -1;

void
mutex_init(void)
{
    /* 共享存储初始化 */
    if ((fd = open(shared_file, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
        err_sys("open error");
    if ((pmutex = mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL)
        err_sys("mmap error");

    /* 互斥量初始化 */
    if ((pthread_mutexattr_init(&attr)) != 0)
        err_sys("pthread_mutexattr_init error");
    if ((pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)) != 0) {
        pthread_mutexattr_destroy(&attr);
        err_sys("pthread_mutexattr_setpshared error");
    }
    /* 参考资料：http://blog.csdn.net/red_knight/article/details/50417898 */
    if ((pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST)) != 0) {
        pthread_mutexattr_destroy(&attr);
        err_sys("pthread_mutexattr_setpshared error");
    }
    if ((pthread_mutex_init(pmutex, &attr)) != 0) {
        pthread_mutexattr_destroy(&attr);
        err_sys("pthread_mutex_init error");
    }
    pthread_mutexattr_destroy(&attr);
}

/* 参考资料：http://blog.csdn.net/red_knight/article/details/50417898 */
/* APUE：347 */
void
mutex_wait(void)  /* P */
{
    int ret; 

    ret = pthread_mutex_lock(pmutex);

    if (ret == EOWNERDEAD) {
        if (pthread_mutex_consistent(pmutex) != 0) {
            pthread_mutex_unlock(pmutex);
            return;
        }
        pthread_mutex_unlock(pmutex);
        pthread_mutex_lock(pmutex);
    } else if (ret == ENOTRECOVERABLE) {
        pthread_mutex_destroy(pmutex);
        munmap(pmutex, sizeof(pthread_mutex_t));
        /* re-init */
        mutex_init();
        ret = pthread_mutex_lock(pmutex);
    }
    if (ret != 0) {
        pthread_mutex_destroy(pmutex);
        munmap(pmutex, sizeof(pthread_mutex_t));
        err_sys("pthread_mutex_lock error");
    }
}

void
mutex_signal(void)    /* V */
{
    if (pthread_mutex_unlock(pmutex) != 0) {
        pthread_mutex_destroy(pmutex);
        munmap(pmutex, sizeof(pthread_mutex_t));
        err_sys("pthread_mutex_unlock error");
    }
}
/*
 * 当调用pthread_mutex_lock获取锁，而该锁被另一个进程持有，但它终止时
 * 并没有对该锁进行解锁，此时线程会阻塞，从pthread_mutex_lock返回的值
 * 为EOWNERDEAD而不是0.
 * APUE P346
 */

/* ？？？
 * 如何在共享内存中设置
 * 参考资料：
 * http://blog.chinaunix.net/uid-26575352-id-3440212.html
 */
