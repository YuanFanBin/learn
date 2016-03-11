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



/* 线程同步：4.自旋锁----->忙等 */
