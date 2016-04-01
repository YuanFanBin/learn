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

/* gcc apue.h apue_err.c figure-11.3.c -lpthread */
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

/*
 * pthread_detach 线程分离，如果对某个线程的终止状态不感新区的话，可以使用
 * pthread_detach函数让操作系统在线程退出时收回它所占用的资源。
 *
 * man pthread_detach样例用法：
 *
 * pthread_detach(pthread_self());
 */

/*
 * 通过查看 man pthread_join 我们可以了解，若不分离线程（pthread_join/pthread_detach/
 * 通过设置pthread_attr_t的相应属性）那么线程终止时，会形成僵死线程（zombie thread），
 * 此概念与僵死进程（zombie process）相似。
 */
