#ifndef __udpcksum_h
#define __udpcksum_h

#include "../../lib/error.h"
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
// #include <netinet/ip_var.h>      // struct udpiphdr{} FreeBSD
#include <netinet/udp.h>
// #include <netinet/udp_var.h>     // struct udpiphdr{} FreeBSD
#include <pcap.h>
#include <sys/socket.h>
#include <sys/types.h>

#define TTL_OUT 64  // outgoing TTL
#define MAXLINE 4096        /* max text line length */

// declare global variables
extern struct sockaddr *dest, *local;
extern socklen_t        destlen, locallen;
extern int              datalink;
extern char            *device;
extern pcap_t          *pd;
extern int              rawfd;
extern int              snaplen;
extern int              verbose;
extern int              zerosum;

// function prototypes
void cleanup(int);
char *next_pcap(int *);
void open_output(void);
void open_pcap(void);
void send_dns_query(void);
void test_udp(void);
void udp_write(char *, int);
struct udpiphdr *udp_read(void);

#endif // __udpcksum_h
