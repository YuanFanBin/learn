#include "apue.h"
#include <pthread.h>

struct foo {
    int a, b, c, d;
};

void
printfoo(const char *s, const struct foo *fp)
{
    printf("%s", s);
    printf(" structure at 0x%lx\n", (unsigned long)fp);
    printf(" foo.a = %d\n", fp->a);
    printf(" foo.b = %d\n", fp->b);
    printf(" foo.c = %d\n", fp->c);
    printf(" foo.d = %d\n", fp->d);
}

void *
thr_fn1(void *arg)
{
    struct foo foo = {1, 2, 3, 4};  /* 线程栈上数据，线程结束后，栈数据可不用。*/

    printfoo("thread 1: \n", &foo);
    pthread_exit((void*)&foo);
}

void *
thr_fn2(void *arg)
{
    printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());
    pthread_exit((void*)0);
}

/* gcc apue.h apue_err.c figure-11.4.c -lpthread */
int
main(void)
{
    int         err;
    pthread_t   tid1, tid2;
    struct foo *fp;

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        err_exit(err, "can't create thread 1");

    err = pthread_join(tid1, (void*)&fp);
    if (err != 0)
        err_exit(err, "can't join with thread 1");

    sleep(1);
    printf("parent starting second thread\n");

    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        err_exit(err, "can't create thread 2");

    sleep(1);
    printfoo("parent:\n", fp);
    exit(0);
}

/*
 * pthread_create, pthread_exit函数的无类型指针参数可以传递的值不止一个，
 * 这个指针可以传递包含复杂信息的结构的地址，但是注意，这个结构所使用的
 * 内存在调用者完成调用以后必须仍然有效的。
 * 例如：
 *   在调用线程上分配了一个结构，那么其他线程在使用这个结构时内存内容可
 *   能已经改变了。
 * 又如：
 *   线程在自己的栈上分配了一个结构，然后把指向这个结构的指针传给
 *   pthread_exit，那么调用pthread_join的线程试图使用该结构时，这个栈有
 *   可能已经被撤销，这块内存也已另作他用。
 */
