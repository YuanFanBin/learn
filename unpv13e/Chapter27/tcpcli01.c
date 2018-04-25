#include "../lib/error.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERV_PORT_STR   "9877"

#ifndef u_char
typedef unsigned char u_char;
#endif

int optind, opterr, optopt;

// sourceroute.c
u_char *inet_srcrt_init(int type);
int inet_srcrt_add(char *hostptr);
// ../Chapter11/host_serv.c
struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype);
// ../Chapter05/str_cli.c
void str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv)
{
    int              c, sockfd, len = 0;
    u_char          *ptr = NULL;
    struct addrinfo *ai;

    if (argc < 2) {
        err_quit("usage: tcpcli01 [ -[gG] <hostname> ... ] <hostname>");
    }
    opterr = 0; // don't want getopt() writing to stderr
    while ((c = getopt(argc, argv, "gG")) != -1) {
        switch (c) {
            case 'g':   // loose source route
                if (ptr) {
                    err_quit("can't use both -g and -G");
                }
                ptr = inet_srcrt_init(0);
                break;
            case 'G':   // strict source route
                if (ptr) {
                    err_quit("can't use both -g and -G");
                }
                ptr = inet_srcrt_init(1);
                break;
            case '?':
                err_quit("unrecognized option: %c", c);
        }
    }
    if (ptr) {
        while (optind < argc - 1) {

        }
    } else if (optind < argc - 1) {
        err_quit("need -g or -G to specify route");
    }
    if (optind != argc - 1) {
        err_quit("missing <hostname>");
    }
    ai = host_serv(argv[optind], SERV_PORT_STR, AF_INET, SOCK_STREAM);
    if ((sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
        err_sys("socket error");
    }
    if (ptr) {
        len = inet_srcrt_add(argv[optind]); // dest at end
    }
    connect(sockfd, ai->ai_addr, ai->ai_addrlen);
    str_cli(stdin, sockfd); // do it all
    exit(0);
}
