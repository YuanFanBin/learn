#include <string.h>     /* memcpy */
#include <errno.h>      /* EAFNOSUPPORT */
#include <arpa/inet.h>  /* inet_aton */
#include <netinet/in.h> /* struct in_addr, AF_INET */

int inet_pton(int family, const char *strptr, void *addrptr)
{
    if (family == AF_INET) {        /* 仅支持IPv4 */
        struct in_addr in_val;

        if (inet_aton(strptr, &in_val)) {
            memcpy(addrptr, &in_val, sizeof(struct in_addr));
            return(1);
        }
        return(0);
    }
    errno = EAFNOSUPPORT;
    return(-1);
}
