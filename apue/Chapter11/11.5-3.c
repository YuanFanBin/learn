#include "apue.h"
#include <pthread.h>

void
cleanup(void *arg)
{
    printf("cleanup: %s\n", (char *)arg);
}

void *
thr_fn1(void *arg)
{
    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup, "thread 1 first handler");
    pthread_cleanup_push(cleanup, "thread 1 second handler");
    printf("thread 1 push complete\n");
    if (arg)
        return((void *)1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return((void *)1);
}

void *
thr_fn2(void *arg)
{
    printf("thread 2 start\n");
    pthread_cleanup_push(cleanup, "thread 2 first handler");
    pthread_cleanup_push(cleanup, "thread 2 second handler");
    printf("thread 2 push complete\n");
    if (arg)
        pthread_exit((void *)2);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void *)2);
}

/* gcc 11.5-3.c apue.h apue_err.c -lpthread */
int
main(void)
{
    int        err;
    pthread_t  tid1, tid2;
    void      *tret;

    err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
    if (err != 0)
        err_exit(err, "can't create thread 1");
    err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
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
 * pthread_clean_push, pthread_clean_pop需成对出现
 * 当线程执行以下动作时，清理函数是由pthread_cleanup_push函数调度的，
 * 调用时只有一个参数arg：
 *   调用pthread_exit时；
 *   响应取消请求时；
 *   用非零参数调用pthread_clean_pop时。
 */
/* pthread_cancel 并不等待线程终止，它仅仅提出请求。*/

/* 进城函数和线程函数之间的相似之处
 * 进城原语  线程原语             描述
 * fork      pthread_create       创建新的控制流
 * exit      pthread_exit         从现有的控制流中退出
 * waitpid   pthread_join         从控制流中得到退出状态
 * atexit    pthread_cancel_push  注册在退出控制流时调用的函数
 * getpid    pthread_self         获取控制流的ID
 * abort     pthread_cancel       请求控制流的非正常退出
 */
