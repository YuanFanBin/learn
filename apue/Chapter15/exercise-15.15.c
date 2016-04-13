#include "apue.h"
#include <fcntl.h>
#include <sys/shm.h>

#define NLOOPS      10
#define SIZE        sizeof(long)    /* size of shared memory area */

static int
update(long *ptr)
{
    return((*ptr)++);       /* return value before increment */
}

/* gcc apue.h apue_err.c figure-15.7.c exercise-15.15.c */
int
main(void)
{
    int     fd, i, counter;
    pid_t   pid;
    void   *area;
    int     shmid;

    if ((fd = open("/dev/zero", O_RDWR)) < 0)
        err_sys("open error");
    /* XSI 共享存储 */
    if ((shmid = shmget(IPC_PRIVATE, SIZE, 0600)) < 0)
        err_sys("shmget error");
    if ((area = shmat(shmid, 0, 0)) == (void *)-1)
        err_sys("shmat error");

    /* 使用figure-15.7.c -- 管道实现 */
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {   /* parent */
        for (i = 0; i < NLOOPS; i += 2) {
            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);

            fprintf(stdout, "pc = %d,", counter);
            TELL_CHILD(pid);
            WAIT_CHILD();
        }
    } else {        /* child */
        for (i = 1; i < NLOOPS; i += 2) {
            WAIT_PARENT();

            if ((counter = update((long *)area)) != i)
                err_quit("child: expected %d, got %d", i, counter);

            fprintf(stdout, "cc = %d,", counter);
            TELL_PARENT(getppid());
        }
    }
    fprintf(stdout, "OK");
    exit(0);
}
