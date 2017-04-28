#include <stdio.h>      /* snprintf */
#include <string.h>     /* strlen */
#include <errno.h>      /* ENOSPC */
#include <netinet/in.h> /* AF_INET */

const char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
    const u_char *p = (const u_char *)addrptr;
    if (family == AF_INET) {        /* 仅支持IPv4 */
        char temp[INET_ADDRSTRLEN];
        snprintf(temp, sizeof(temp), "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
        if (strlen(temp) >= len) {
            errno = ENOSPC;
            return(NULL);
        }
        strcpy(strptr, temp);
        return(strptr);
    }
    errno = EAFNOSUPPORT;
    return(NULL);
}
