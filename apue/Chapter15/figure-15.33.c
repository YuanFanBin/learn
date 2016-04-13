#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>

#define NLOOPS      1000
#define SIZE        sizeof(long)    /* size of shared memory area */

static int
update(long *ptr)
{
    return((*ptr)++);       /* return value before increment */
}

/* gcc apue.h apue_err.c figure-15.7.c figure-15.33.c */
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

    /* 使用figure-15.7.c -- 管道实现 */
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {   /* parent */
        for (i = 0; i < NLOOPS; i += 2) {
            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);

            TELL_CHILD(pid);
            WAIT_CHILD();
        }
    } else {        /* child */
        for (i = 1; i < NLOOPS; i += 2) {
            WAIT_PARENT();

            if ((counter = update((long *)area)) != i)
                err_quit("child: expected %d, got %d", i, counter);

            TELL_PARENT(getppid());
        }
    }
    fprintf(stdout, "OK");
    exit(0);
}
