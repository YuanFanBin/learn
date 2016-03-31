#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(id)  (((unsigned long)id)%NHASH)

struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
    int              f_count; /* protected by hashlock */
    pthread_mutex_t  f_lock;
    int              f_id;
    struct foo      *f_next; /* protected by hashlock */
    /* ... more stuff here ... */
};

struct foo *
foo_alloc(int id) /* allocate the object */
{
    struct foo *fp;
    int         idx;

    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        fp->f_count = 1;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return(NULL);
        }
        idx = HASH(id);
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];
        fh[idx] = fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);
        /* ... continue initialization ... */
        pthread_mutex_unlock(&fp->f_lock);
    }
    return(fp);
}

void
foo_hold(struct foo *fp) /* add a reference to the object */
{
    pthread_mutex_lock(&hashlock); /* 锁不一样了 */
    fp->f_count++;
    pthread_mutex_unlock(&hashlock);
}

struct foo *
foo_hold(int id) /* find an existing object */
{
    struct foo *fp;

    pthread_mutex_lock(&hashlock);
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
        if (fp->f_id == id) {
            fp->f_count++;
            break;
        }
    }
    pthread_mutex_unlock(&hashlock);
    return(fp);
}

void
foo_rele(struct foo *fp) /* release a reference to the object */
{
    struct foo *tfp;
    int         idx;

    pthread_mutex_lock(&hashlock);
    if (--fp->f_count == 0) { /* last reference, remove from list */
        idx = HASH(fp->f_id);
        tfp = fh[idx];
        if (tfp == fp) {
            fh[idx] = fp->f_next;
        } else {
            while (tfp->f_next != fp)
                tfp = tfp->f_next;
            tfp->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&hashlock);
    }
}
/* 样例中使用了粗粒度的锁 */

/* 线程同步：1.互斥量 */

/* 与 Figure-11.11.c 的程序相比，本文件的程序简单的得多。两种用途使用相同的锁时，
 * 围绕散列列表和引用计数的锁的排序问题就不存在了。
 *
 * 多线程的软件设计涉及这两者之间的折中。
 *
 * 如果锁的粒度太粗，就会出现很多线程阻塞等待相同的锁，这可能并不能改善并发性。
 *
 * 如果锁的粒度太细，那么过多的锁开销会使系统性能受到影响，而且代码变得更复杂。
 *
 * 作为一个程序员，需要在满足锁需求的情况下，在代码复杂性和性能之间找到正确的平衡。
 */
