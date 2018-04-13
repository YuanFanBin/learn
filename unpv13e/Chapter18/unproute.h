#ifndef __unproute_h
#define __unproute_h

#include <net/if.h>     // if_msghdr{}
// TODO: BSD-specific API
// https://stackoverflow.com/questions/1520649/what-package-do-i-need-to-install-for-using-routing-sockets
#include <net/if_dl.h>  // sockaddr_sdl{}
#include <net/route.h>  // RTA_xxx constants
#include <sys/param.h>

#ifdef HAVE_SYS_SYSCTL_H
#include <sys/sysctl.h> // sysctl()
#endif

// function prototypes
void get_rtaddrs(int, struct sockaddr *, struct sockaddr **);
char *net_rt_iflist(int, int, size_t *);
char *net_rt_dump(int, int, size_t *);
const char *sock_masktop(struct sockaddr *, socklen_t);

#endif // __unproute_h
