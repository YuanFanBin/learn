#include "../lib/error.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16  /* "ddd.ddd.ddd.ddd\0"
                                1234567890123456 */
#endif

int main(int argc, char **argv) {
    char           *ptr, **pptr;
    char            str[INET_ADDRSTRLEN];
    struct hostent *hptr;

    while (--argc > 0) {
        ptr = *++argv;
        if ((hptr = gethostbyname(ptr)) == NULL) {
            err_msg("gethostbyname error for host: %s: %s", ptr, hstrerror(h_errno));
            continue;
        }
        printf("official hostname: %s\n", hptr->h_name);

        for (pptr = hptr->h_aliases; *pptr != NULL; pptr++) {
            printf("\talias: %s\n", *pptr);
        }

        switch (hptr->h_addrtype) {
            case AF_INET:
                pptr = hptr->h_addr_list;
                for (; *pptr != NULL; pptr++) {
                    printf("\t address: %s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
                }
            default:
                err_ret("unknown address type");
                break;
        }
    }
    exit(0);
}

// $ gcc hostent.c ../lib/error.h ../lib/error.c -o hostent
