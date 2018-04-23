#include "../lib/error.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stropts.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int sockfd;

#define QSIZE       8       // size of input queue
#define MAXDG       4096    // max datagram size

typedef struct {
    void            *dg_data;       // ptr to actual datagram
    size_t           dg_len;        // length of datagram
    struct sockaddr *dg_sa;         // ptr to sockaddr{} w/client's address
    socklen_t        dg_salen;      // length of sockaddr{}
} DG;

static DG dg[QSIZE];            // queue of datagrams to process
static long cntread[QSIZE+1];   // diagnostic counter

static int iget;            // next one for main loop to process
static int iput;            // next one for signal handler to read into
static int nqueue;          // # on queue for main loop to process
static socklen_t clilen;    // max length of sockaddr{}

static void sig_io(int signo);
static void sig_hup(int signo);

void dg_echo(int sockfd_arg, struct sockaddr *pcliaddr, socklen_t clilen_arg)
{
    int         i;
    const int   on = 1;
    sigset_t    zeromask, newmask, oldmask;

    sockfd = sockfd_arg;
    clilen = clilen_arg;

    for (i = 0; i < QSIZE; i++) {   // init queue of buffers
        dg[i].dg_data = malloc(MAXDG);
        dg[i].dg_sa = malloc(clilen);
        dg[i].dg_salen = clilen;
    }
    iget = iput = nqueue = 0;

    signal(SIGHUP, sig_hup);
    signal(SIGIO, sig_io);
    fcntl(sockfd, F_SETOWN, getpid());
    ioctl(sockfd, FIOASYNC, &on);
    ioctl(sockfd, FIONBIO, &on);

    sigemptyset(&zeromask);
    sigemptyset(&oldmask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGIO);

    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    for ( ; ; ) {
        while (nqueue == 0) {
            sigsuspend(&zeromask);  // wait for datagram to process
        }
        // unblock SIGIO
        sigprocmask(SIG_SETMASK, &oldmask, NULL);
        sendto(sockfd, dg[iget].dg_data, dg[iget].dg_len, 0, dg[iget].dg_sa, dg[iget].dg_salen);
        if (++iget >= QSIZE) {
            iget = 0;
        }
        // block SIGIO
        sigprocmask(SIG_BLOCK, &newmask, &oldmask);
        nqueue--;
    }
}

// sig_io SIGIO信号处理函数，当连接有数据到达时，此函数运行，通过使用 QSIZE 来
//        控制接收队列大小
static void sig_io(int signo)
{
    ssize_t     len;
    int         nread;
    DG         *ptr;

    for (nread = 0; ; ) {
        if (nqueue >= QSIZE) {
            err_quit("receive overflow");
        }

        ptr = &dg[iput];
        ptr->dg_salen = clilen;
        len = recvfrom(sockfd, ptr->dg_data, MAXDG, 0, ptr->dg_sa, &ptr->dg_salen);
        if (len < 0) {
            if (errno == EWOULDBLOCK) {
                break;  // all done; no more queued to read
            }
            err_sys("recvfrom error");
        }
        ptr->dg_len = len;

        nread++;
        nqueue++;
        if (++iput >= QSIZE) {
            iput = 0;
        }
    }
    cntread[nread]++;   // historgram of # datagrams read per signal
}

static void sig_hup(int signo)
{
    int i;

    for (i = 0; i <= QSIZE; i++) {
        printf("cntread[%d] = %ld\n", i, cntread[i]);
    }
}
