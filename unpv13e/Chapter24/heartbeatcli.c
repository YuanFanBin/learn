#include "../lib/error.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

static int servfd;
static int nsec;            // #seconds between each alarm
static int maxnprobes;      // #probes w/no response before quit
static int nprobes;         // #probes since last server response
static void sig_urg(int signo), sig_alrm(int signo);

void heartbeat_cli(int servfd_arg, int nsec_arg, int maxnprobes_arg)
{
    servfd = servfd_arg;        // set globals for signal handlers
    if ((nsec = nsec_arg) < 1) {
        nsec = 1;
    }
    if ((maxnprobes = maxnprobes_arg) < nsec) {
        maxnprobes = nsec;
    }
    nprobes = 0;

    signal(SIGURG, sig_urg);
    fcntl(servfd, F_SETOWN, getpid());

    signal(SIGALRM, sig_alrm);
    alarm(nsec);
}

static void sig_urg(int signo)
{
    int  n;
    char c;

    if ((n = recv(servfd, &c, 1, MSG_OOB)) < 0) {
        if (errno != EWOULDBLOCK) {
            err_sys("recv error");
        }
    }
    nprobes = 0;    // rset counter
    printf("SIGURG(%d)\n", signo);
}

static void sig_alrm(int signo)
{
    if (++nprobes > maxnprobes) {
        fprintf(stderr, "server is unreachable\n");
        exit(0);
    }
    send(servfd, "1", 1, MSG_OOB);
    alarm(nsec);
    printf("SIGALRM(%d)\n", signo);
}
