#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char        portstr[8];
    static char str[128];   /* Unix domain is largest */

    switch (sa->sa_family) {
        case AF_INET: {
            struct sockaddr_in *sin = (struct sockaddr_in *) sa;

            if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL) {
                return(NULL);
            }
            if (ntohs(sin->sin_port) != 0) {
                snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
                strcat(str, portstr);
            }
            return(str);
        }
    }
    return(NULL);
}
