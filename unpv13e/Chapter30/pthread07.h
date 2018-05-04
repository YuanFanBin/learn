#ifndef __pthread07_h
#define __pthread07_h
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef struct {
    pthread_t thread_tid;   // thread ID
    long thread_count;      // # connections handled
} Thread;
Thread *tptr;   // array of Thread structures; calloc'ed

int             listenfd, nthreads;
socklen_t       addrlen;
pthread_mutex_t mlock;

#endif // __pthread07_h
