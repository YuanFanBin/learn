/* sem.c */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include "section-12.4.1-sem.h"

/* 创建信号量 */
semaphore_t *
semaphore_create(char *semaphore_name)
{
    int                  fd;
    semaphore_t         *semap;
    pthread_mutexattr_t  psharedm;  /* 共享互斥量 */
    pthread_condattr_t   psharedc;  /* 共享条件变量 */

    fd = open(semaphore_name, O_RDWR | O_CREAT | O_EXCL, 0666);
    if (fd < 0)
        return(NULL);
    (void) ftruncate(fd, sizeof(semaphore_t));  /* 把文件截断为指定长度 */
    /* 设置为进程共享属性 */
    (void) pthread_mutexattr_init(&psharedm);
    (void) pthread_mutexattr_setpshared(&psharedm, PTHREAD_PROCESS_SHARED);
    /* 设置为进程共享属性 */
    (void) pthread_condattr_init(&psharedc);
    (void) pthread_condattr_setpshared(&psharedc, PTHREAD_PROCESS_SHARED);

    /* 存储映射I/O(section 14.8)，将文件映射至内存，用于共享信号量(sem) */
    semap = (semaphore_t *) mmap(NULL, sizeof(semaphore_t),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    /* 初始化互斥量&条件变量 */
    (void)pthread_mutex_init(&semap->lock, &psharedm);
    (void)pthread_cond_init(&semap->nonzero, &psharedc);
    return(semap);
}

/* 打开一个信号量 */
semaphore_t *
semaphore_open(char *semaphore_name)
{
    int          fd;
    semaphore_t *semap;

    fd = open(semaphore_name, O_RDWR, 0666);
    if (fd < 0)
        return(NULL);
    semap = (semaphore_t *)mmap(NULL, sizeof(semaphore_t),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return(semap);
}

/* 增加信号量值 */
void
semaphore_post(semaphore_t *semap)
{
    pthread_mutex_lock(&semap->lock);
    if (semap->count == 0)
        pthread_cond_signal(&semap->nonzero);
    semap->count++;
    pthread_mutex_unlock(&semap->lock);
}

/* 减少信号量值 */
void
semaphore_wait(semaphore_t *semap)
{
    pthread_mutex_lock(&semap->lock);
    while(semap->count == 0)
        pthread_cond_wait(&semap->nonzero, &semap->lock);
    semap->count--;
    pthread_mutex_unlock(&semap->lock);
}

void
semaphore_close(semaphore_t *semap)
{
    munmap((void *)semap, sizeof(semaphore_t));
}

/* man pthread_mutex_attr_init */

/* 
 * 实现一个信号量机制。
 */
