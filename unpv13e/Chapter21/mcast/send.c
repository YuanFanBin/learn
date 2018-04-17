#include "../../lib/error.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

#define SENDRATE    5       // send one datagram every five seconds
#define MAXLINE     4096    // max text line length

void send_all(int sendfd, struct sockaddr *sadest, socklen_t salen)
{
    char            line[MAXLINE];     // hostname and process ID
    struct utsname  myname;

    if (uname(&myname) < 0) {
        err_sys("uname error");
    }
    snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());
    for ( ; ; ) {
        sendto(sendfd, line, strlen(line), 0, sadest, salen);
        sleep(SENDRATE);
    }
}
