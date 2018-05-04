#ifndef __mcast_h
#define __mcast_h

#include <errno.h>
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_IPV6
#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifndef u_int
typedef unsigned int u_int;
#endif

#ifndef u_char
typedef unsigned char u_char;
#endif

int mcast_join(int sockfd, const struct sockaddr *grp, socklen_t grplen,
               const char *ifname, u_int ifindex);

int mcast_leave(int sockfd, const struct sockaddr *grp, socklen_t grplen);

int mcast_block_source(int sockfd,
                       const struct sockaddr *src, socklen_t srclen,
                       const struct sockaddr *grp, socklen_t grplen);

int mcast_unblock_source(int sockfd,
                         const struct sockaddr *src, socklen_t srclen,
                         const struct sockaddr *grp, socklen_t grplen);

int mcast_join_source_group(int sockfd,
                            const struct sockaddr *src, socklen_t srclen,
                            const struct sockaddr *grp, socklen_t grplen,
                            const char *ifname, u_int ifindex);

int mcast_leave_source_group(int sockfd,
                             const struct sockaddr *src, socklen_t srclen,
                             const struct sockaddr *grp, socklen_t grplen);

int mcast_set_if(int sockfd, const char *ifname, u_int ifindex);

int mcast_set_loop(int sockfd, int onoff);

int mcast_set_ttl(int sockfd, int ttl);

int mcast_get_if(int sockfd);

int mcast_get_loop(int sockfd);

int mcast_get_ttl(int sockfd);

int family_to_level(int family);

int sockfd_to_family(int sockfd);

#endif // __mcast_h
