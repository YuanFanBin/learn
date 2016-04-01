#include "apue.h"
#include <pthread.h>

int
makethread(void *(*fn)(void *), void *arg)
{
    int             err;
    pthread_t       tid;
    pthread_attr_t  attr;

    err = pthread_attr_init(&attr);
    if (err != 0)
        return(err);
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err == 0)
        err = pthread_create(&tid, NULL, fn, arg);
    pthread_attr_destroy(&attr);    /* 这里忽略了返回状态，可能会造成内存泄露 */
    return(err);
}

/*
 * 以分离状态创建一个线程，后续无需使用pthread_detach/pthread_join回收/获取资源，
 * 退出状态。
 */
