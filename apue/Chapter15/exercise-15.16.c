#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/sem.h>

#define NLOOPS      100
#define SIZE        sizeof(long)    /* size of shared memory area */

/* 通过 sudo ipcs 查看当前正在使用的ipc id */
/* 通过 sudo ipcrm 删除不再使用的ipc id */
#define SEMKEY1 (key_t)0x2006
#define SEMKEY2 (key_t)0x2007
#define SEMKEY3 (key_t)0x2008

static int
update(long *ptr)
{
    return((*ptr)++);       /* return value before increment */
}

/* 参考资料：http://blog.chinaunix.net/uid-23193900-id-3221978.html */
union semun {
    int             val;
    struct semid_ds *buf;
    unsigned short  *array;
};

/* 信号量初始化 */
static int
sem_init(key_t key, int inival)
{
    union semun sun;
    int         semid;

    if ((semid = semget(key, 1, 0660 | IPC_CREAT | IPC_EXCL)) < 0)
        err_sys("semget error");
    sun.val = inival;
    if (semctl(semid, 0, SETVAL, sun) < 0)
        err_sys("semctl error");

    return semid;
}

static int
sem_destroy(int semid)
{
    return(semctl(semid, 0, IPC_RMID, NULL));
}

static int
P(int semid)
{
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}

static int
V(int semid)
{
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}

/* gcc apue.h apue_err.c exercise-15.16.c */
int
main(void)
{
    int     fd, i, counter;
    pid_t   pid;
    void   *area;

    if ((fd = open("/dev/zero", O_RDWR)) < 0)
        err_sys("open error");
    /* 存储映射 */
    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
        == MAP_FAILED)
        err_sys("mmap error");
    close(fd);      /* can close /dev/zero now that it's mapped */

    /* XSI信号量 */

    int mutex = sem_init(SEMKEY1, 1);
    int producter = sem_init(SEMKEY2, 0);
    int consumer = sem_init(SEMKEY3, 1);

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {   /* parent */
        for (i = 0; i < NLOOPS; i += 2) {
            P(consumer);
            P(mutex);
            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);
            V(mutex);
            V(producter);
        }
        waitpid(pid, NULL, 0);
        sem_destroy(mutex);
        sem_destroy(producter);
        sem_destroy(consumer);
    } else {        /* child */
        for (i = 1; i < NLOOPS; i += 2) {
            P(producter);
            P(mutex);
            if ((counter = update((long *)area)) != i)
                err_quit("child: expected %d, got %d", i, counter);
            V(mutex);
            V(consumer);
        }
    }
    printf("ok");
    exit(0);
}
