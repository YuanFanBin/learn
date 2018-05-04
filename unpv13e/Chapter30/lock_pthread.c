#include "../lib/error.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static pthread_mutex_t *mptr;   // actual mutex will be in shared memory

void my_lock_init(char *pathname)
{
    int fd, e;
    pthread_mutexattr_t mattr;

    fd = open("/dev/zero", O_RDWR, 0);

    mptr = mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    if ((e = pthread_mutexattr_init(&mattr)) != 0) {
        errno = e;
        err_sys("pthread_mutexattr_init error");
    }
    if ((e = pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED)) != 0) {
        errno = e;
        err_sys("pthread_mutexattr_setpshared error");
    }
    if ((e = pthread_mutex_init(mptr, &mattr)) != 0) {
        errno = e;
        err_sys("pthread_mutex_init error");
    }
}

void my_lock_wait()
{
    int e;
    if ((e = pthread_mutex_lock(mptr)) != 0) {
        errno = e;
        err_sys("pthread_mutex_lock error");
    }
}

void my_lock_release()
{
    int e;
    if ((e = pthread_mutex_unlock(mptr)) != 0) {
        errno = e;
        err_sys("pthread_mutex_unlock error");
    }
}
