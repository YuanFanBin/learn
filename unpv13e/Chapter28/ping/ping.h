#ifndef __ping_h
#define __ping_h

#include "../../lib/error.h"
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#ifdef IPV6
#include <netinet/icmp6.h>
#include <netinet/ip6.h>
#endif


#define BUFSIZE     1500

char sendbuf[BUFSIZE];

// globals
int     datalen;    // # bytes of data following ICMP header
char   *host;
int     nsent;
pid_t   pid;
int     sockfd;
int     verbose;

// function prototypes
void init_v6(void);
void proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void proc_v6(char *, ssize_t, struct msghdr *, struct timeval *);
void send_v4(void);
void send_v6(void);
void readloop(void);
void sig_alrm(int);
void tv_sub(struct timeval *, struct timeval *);

struct proto {
    void (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
    void (*fsend)(void);
    void (*finit)(void);
    struct sockaddr *sasend;    // sockaddr{} for send, from getaddrinfo
    struct sockaddr *sarecv;    // sockaddr{} for receiving
    socklen_t        salen;            // length of sockaddr{}
    int              icmpproto;
} *pr;

#endif // __ping_h
