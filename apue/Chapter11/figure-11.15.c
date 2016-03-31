#include <pthread.h>

struct msg {
    struct msg *m_next;
    /* ... more stuff here ... */
};

struct msg *workq;

pthread_cond_t  qready = PTHREAD_COND_INITIALIZER;

ptrhead_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

/* 如何使用条件变量和互斥量对线程进行同步 */

void
process_msg(void)
{
    struct msg *mp;

    for (;;) {
        pthread_mutex_lock(&qlock);
        while (workq == NULL)
            pthread_cond_wait(&qread, &qlock); /* 内部会对qlock解锁, 返回时再锁住 */
        mp = workq;
        workq = mp->next;
        pthread_mutex_unlock(&qlock);
        /* now process the message mp */
    }
}

void
enqueue_msg(struct msg *mp)
{
    pthread_mutex_lock(&qlock);
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
}

/* 线程同步：3.条件变量 */
/* 
 * 线程同步的各种方法：
 * https://en.wikipedia.org/wiki/Synchronization_(computer_science)#Pthreads_synchronization
 */

/*
 * 传递给 `pthread_cond_wait` 的互斥量对条件进行保护。调用者把锁住的互斥量传给函数，
 * 函数然后自动把调用线程放到等待条件的线程列表上，对互斥量解锁。这就关闭了条件检查
 * 和线程进入休眠状态等待条件改变这两个操作之间的时间通道，这样线程就不会错过条件的
 * 任何变化。`pthread_cond_wait` 返回时，互斥量再次被锁住。
 */



/* 线程同步：4.自旋锁----->忙等 */
/* 
 * 线程同步的各种方法：
 * https://en.wikipedia.org/wiki/Synchronization_(computer_science)#Pthreads_synchronization
 */

/*
 * 自旋锁与互斥量类似，但它不是通过休眠使进程阻塞，而是在获取锁之前一直处于忙等（自旋）
 * 阻塞状态。
 *
 * 自旋锁可用于以下情况：
 * 锁被持有的时间短，而且线程并不希望在重新调度上花费太多的成本。
 */
