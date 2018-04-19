#ifndef __pktinfo_h
#define __pktinfo_h

#include <netinet/in.h>

struct unp_in_pktinfo {
    struct in_addr  ipi_addr;       // destination IPv4 address
    int             ipi_ifindex;   // received interface index
};

#endif  // __pktinfo_h
