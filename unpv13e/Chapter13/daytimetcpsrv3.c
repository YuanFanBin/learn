#include "../lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAXLINE 4096 /* max text line length */

void daemon_inetd(const char *pname, int facility);
char *Sock_ntop(const struct sockaddr *sa, socklen_t salen);

int main(int argc, char **argv)
{
    socklen_t        len;
    struct sockaddr *cliaddr;
    char             buff[MAXLINE];
    time_t           ticks;

    daemon_inetd(argv[0], 0);

    cliaddr = (struct sockaddr *) malloc (sizeof(struct sockaddr_storage));
    len = sizeof(struct sockaddr_storage);
    if (getpeername(0, cliaddr, &len) == -1) {
        err_sys("getpeername error");
    }
    err_msg("connection from %s", Sock_ntop(cliaddr, len));

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    write(0, buff, strlen(buff));

    close(0);
    exit(0);
}

// $ sudo vim /etc/services # add services
// $ tail -n 2 /etc/services
// unpv13e         58888/tcp
// unpv13e         58888/udp
//
// $ sudo vim /etc/inet.conf    # not found (ArchLinux)
// unpv13e stream tcp nowait fanbin /home/fanbin/github/learn/unpv13e/Chapter13/daytimetcpsrv3 daytimetcpsrv3
//
// $ gcc daytimetcpsrv3.c daemon_inetd.c ../lib/sock_ntop.c ../lib/error.c -o daytimetcpsrv3
