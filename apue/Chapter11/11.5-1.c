#include "apue.h"
#include <pthread.h>

void *
thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    return((void*)1);
}

void *
thr_fn2(void *arg)
{
    printf("thread 2 exiting\n");
    pthread_exit((void*)2);
}

/* gcc 11.5-1.c apue.h apue_err.c -lpthread */
int
main(void)
{
    int        err;
    pthread_t  tid1, tid2;
    void      *tret;

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        err_exit(err, "can't create thread 1");
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        err_exit(err, "can't create thread 2");
    err = pthread_join(tid1, &tret);
    if (err != 0)
        err_exit(err, "can't join with thread 1");
    printf("thread 1 exit code %ld\n", (long)tret);
    err = pthread_join(tid2, &tret);
    if (err != 0)
        err_exit(err, "can't join with thread 2");
    printf("thread 2 exit code %ld\n", (long)tret);
    exit(0);
}
/* pthread_join的使用线程将会一直阻塞，直到指定的线程调用pthread_exit，
 * 从启动历程中返回或被取消。
 */
/*   单个线程可以通过3中方式退出，因此可以在不终止整个进程的情况下，停止它
 * 的控制流。
 *   1.线程可以简单地从启动例程中返回，返回值是线程的退出码。
 *   2.线程可以被同一进程中的其他线程取消。
 *   3.线程调用pthread_exit。
 */
