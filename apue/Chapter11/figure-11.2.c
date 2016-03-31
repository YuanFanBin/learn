#include "apue.h"
#include <pthread.h>

pthread_t ntid;

void
printids(const char *s)
{
    pid_t     pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
           (unsigned long)tid, (unsigned long)tid);
}

void *
thr_fn(void *arg)
{
    printids("new thread: ");
    return((void*)0);
}

/* gcc apue.h apue_err.c figure-11.2.c -lpthread */
/* 进程ID相同，线程ID不同 */
int
main(void)
{
    int err;
    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0)
        err_exit(err, "can't create thread");
    printids("main thread:");
    sleep(1);
    exit(0);
}
/* 有两个特别之处，需要处理主线程和新线程之间的竞争。
 *   1.主线程需要休眠，如果主线程不休眠，它就可能会退出，这样新线程还没有
 * 机会运行，整个进程可能就已经终止了。（依赖系统的线程实现及调度算法）
 *   2.新线程是通过调用pthread_self函数获取自己的线程ID的，而不是从共享内
 * 存中读出的，或者从线程的启动例程中以参数的形式接收到的。
 */
