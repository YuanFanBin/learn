#include "icmpd.h"
#include <stdio.h>

int readable_listen(void)
{
    int i, connfd;
    socklen_t clilen;

    clilen = sizeof(cliaddr);
    if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
        err_sys("accept error");
    }

    // find first avaliable client[] structure
    for (i = 0; i < FD_SETSIZE; i++) {
        if (client[i].connfd < 0) {
            client[i].connfd = connfd;  // save descriptor
            break;
        }
    }

    if (i == FD_SETSIZE) {
        close(connfd);      // can't handle new client
        return(--nready);   // rudely close the new connection
    }
    printf("new connection, i = %d, connfd = %d\n", i, connfd);

    FD_SET(connfd, &allset);    // add new descriptro to set
    if (connfd > maxfd) {
        maxfd = connfd;     // for select()
    }
    if (i > maxi) {
        maxi = i;       // max index in client[] array
    }
    return(--nready);
}
