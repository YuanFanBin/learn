#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

pid_t child_make(int i, int listenfd, int addrlen)
{
    pid_t pid;
    void  child_main(int, int, int);

    if ((pid = fork()) > 0) {
        return(pid);    // parent
    }
    child_main(i, listenfd, addrlen);   // never returns
}

void child_main(int i, int listenfd, int addrlen)
{
    int              connfd;
    void             web_child(int);
    socklen_t        clilen;
    struct sockaddr *cliaddr;
    extern long     *cptr;

    cliaddr = (struct sockaddr *) malloc(addrlen);

    printf("child %ld starting\n", (long) getpid());
    for ( ; ; ) {
        clilen = addrlen;
        connfd = accept(listenfd, cliaddr, &clilen);
        cptr[i]++;

        web_child(connfd);  // process the request
        close(connfd);
    }
}
