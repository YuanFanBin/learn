#include "apue.h"
#include <pthread.h>
#include <time.h>

/* 参考资料 http://blog.csdn.net/langeldep/article/details/6427780 */
/* or $ man clock_gettime  # Link with -lrt */
/* 
 * clock_gettime并不是所有平台都支持，APUE 11.6.6(P333)有通过gettimtofday
 * 实现的timespec时间
 */

/* gcc apue.h apue_err.c figure-11.13.c -lpthread -lrt */
int
main(void)
{
    int              err;
    struct timespec  tout;
    struct tm       *tmp;
    char             buf[64];
    pthread_mutex_t  lock = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&lock);
    printf("mutex is locked\n");
    clock_gettime(CLOCK_REALTIME, &tout);  /* 编译时需要 -lrt */
    tmp = localtime(&tout.tv_sec);
    strftime(buf, sizeof(buf), "%r", tmp);
    printf("current time is now %s\n", buf);
    tout.tv_sec += 10; /* 10 seconds from now */
    /* caution: this could lead to deadlock */
    err = pthread_mutex_timedlock(&lock, &tout);
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, sizeof(buf), "%r", tmp);
    printf("the time is now %s\n", buf);
    if (err == 0)
        printf("mutex locked again!\n");
    else
        printf("can't lock mutex again:%s\n", strerror(err));
    exit(0);
}

/* 使用pthread_mutex_timedlock避免永久阻塞 */

/* 线程同步：1.互斥量 */
/* 
 * 线程同步的各种方法：
 * https://en.wikipedia.org/wiki/Synchronization_(computer_science)#Pthreads_synchronization
 */
