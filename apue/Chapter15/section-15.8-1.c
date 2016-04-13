/* 15.8 - 实例：信号量，记录锁和互斥量的时间比较 */
/* 1.信号量 */
#include "apue.h"
#include <sys/sem.h>

static key_t key = 0x001124;
static int semid;
/* 使用SEM_UNDO 防止在未释放资源条件下进程终止的情况 */
static struct sembuf p[1] = {0, -1, SEM_UNDO};
static struct sembuf v[1] = {0, 1, SEM_UNDO};

void
sem_init(void)
{
    if ((semid = semget(ftok("/temp/semget", '1'), 1, IPC_CREAT | IPC_EXCL)) < 0)
        err_sys("semget error");
    if (semop(semid, v, 1) == -1)
        err_sys("semop error");
}

void
sem_wait(void) /* P */
{
    if (semop(semid, p, 1) == -1)
        err_sys("semop error");
}

void
sem_signal(void) /* v */
{
    if (semop(semid, v, 1) == -1)
        err_sys("semop error");
}
/* 时间 figure-8.5.c */
