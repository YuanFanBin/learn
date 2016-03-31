#include <stdlib.h>
#include <pthread.h>

struct job {
    struct job *j_next;
    struct job *j_prev;
    pthread_t   j_id; /* tells which thread handles this job */
    /* ... more stuff here ... */
};

/*
 * Initialize a queue.
 */
int
queue_init(struct queue *qp)
{
    int err;
    qp->q_head = NULL;
    if (err != 0)
        return(err);
    /* ... continue initialization ... */
    return(0);
}

/*
 * Insert a job at the head of the queue.
 */
void
job_insert(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if (qp->q_head != NULL)
        qp->q_head->j_prev = jp;
    else
        qp->q_tail = jp; /* list was empty */
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Append a job on the tail of the queue.
 */
void
job_append(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = NULL;
    jp->j_prev = qp->q_tail;
    if (qp->tail != NULL)
        qp->tail->j_next = jp;
    else
        qp->q_head = jp; /* list was empty */
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Remove the given job from a queue.
 */
void
job_remove(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head) {
        qp->q_head = jp->j_next;
        if (qp->q_tail == jp)
            qp->q_tail = NULL;
        else
            jp->j_next->j_prev = jp->j_prev;
    } else if (jp == qp->q_tail) {
        qp->q_tail = jp->j_prev;
        jp->j_next->j_prev = jp->j_prev;
    } else {
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Find a job for the given thread ID.
 */
struct job *
job_find(struct queue *qp, pthread_t id)
{
    struct job *jp;

    if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
        return(NULL);

    for (jp = qp->head; jp != NULL; jp = jp->j_next)
        if (pthread_equal(jp->j_id, id))
            break;
    
    pthread_rwlock_unlock(&qp->q_lock);
    return(jp);
}

/* P328, 11.6.4 */

/* 线程同步：2.读写锁 */
/* 
 * 线程同步的各种方法：
 * https://en.wikipedia.org/wiki/Synchronization_(computer_science)#Pthreads_synchronization
 */

/*
 * 读写锁非常适合于对数据结构读的次数远大于写的情况。
 *
 *
 * 在释放读写锁占用的内存之前，需要调用 `pthread_rwlock_destroy` 做清理工作。
 *
 * 如果 `pthread_rwlock_init` 为读写锁分配了资源，`pthread_rwlock_destroy` 将释放这些资源。
 *
 * 如果在调用 `pthread_rwlock_destroy` 之前就释放了读写锁占用的内存空间，那么分配给这个锁
 * 的资源就会丢失。
 */
