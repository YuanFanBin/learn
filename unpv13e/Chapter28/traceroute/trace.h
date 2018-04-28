#ifndef __trace_h
#define __trace_h

#include "../../lib/error.h"
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <sys/time.h>
#ifdef IPV6
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#endif

#define BUFSIZE         1500
#define NI_MAXHOST      1025

struct rec {    // format of outgoing UDP data
    u_short         rec_seq;    // sequence number
    u_short         rec_ttl;    // TTL packet left with
    struct timeval  rec_tv;     // time packet left
};

// globals
char recvbuf[BUFSIZE];
char sendbuf[BUFSIZE];

int      datalen;    // # bytes of data following ICMP header
char    *host;
u_short  sport, dport;
int      nsent;      // add 1 for each sendto()
pid_t    pid;
int      probe, nprobes;
int      sendfd, recvfd;
int      ttl, max_ttl;
int      verbose;

// function prototypes
const char *icmpcode_v4(int);
const char *icmpcode_v6(int);
int recv_v4(int, struct timeval *);
int recv_v6(int, struct timeval *);
void sig_alrm(int);
void traceloop(void);
void tv_sub(struct timeval *, struct timeval *);

struct proto {
    const char      *(*icmpcode)(int);
    int              (*recv)(int, struct timeval *);
    struct sockaddr   *sasend;      // sockaddr{} for send, from getaddrinfo
    struct sockaddr   *sarecv;      // sockaddr{} for receiving
    struct sockaddr   *salast;      // last sockaddr{} for receiving
    struct sockaddr   *sabind;      // sockaddr{} for binding source port
    socklen_t          salen;       // length of sockaddr{}s
    int                icmpproto;   // IPPROTO_xxx value for ICMP
    int                ttllevel;    // setsockopt() level to set TTL
    int                ttloptname;  // setsockopt() name to set TTL
} *pr;

#endif
