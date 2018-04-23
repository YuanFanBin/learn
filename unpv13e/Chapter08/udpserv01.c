#include "../lib/error.h"
#include <netinet/in.h> /* sockaddr_in */
#include <strings.h>
#include <sys/socket.h>

#define SERV_PORT 9877 /* TCP and UDP client-servers */

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen);

int main(int argc, char **argv) {
    int                sockfd;
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        err_sys("bind error");
    }
    dg_echo(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
}

// #1
// $ gcc udpserv01.c ../lib/error.c ../lib/error.h dg_echo.c -o udpsrv
// $ netcat -uv 127.0.0.1 9877
// hello
// hello
// #2
// $ gcc udpserv01.c ../lib/error.c ../lib/error.h dg_echoloop1.c -o udpsrv
// #3
// $ gcc udpserv01.c ../lib/error.c ../lib/error.h dg_echoloop2.c -o udpsrv
