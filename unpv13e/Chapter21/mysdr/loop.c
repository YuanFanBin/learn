#include "../../lib/error.h"
#include "mysdr.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFSIZE    8192        /* buffer size for reads and writes */

char *sock_ntop(const struct sockaddr *sa, socklen_t salen);

void loop(int sockfd, socklen_t salen)
{
    socklen_t        len;
    ssize_t          n;
    char            *p;
    struct sockaddr *sa;
    struct sap_packet {
        uint32_t sap_header;
        uint32_t sap_src;
        char sap_data[BUFFSIZE];
    } buf;

    sa = (struct sockaddr *) malloc(salen);
    for ( ; ; ) {
        len = salen;
        n = recvfrom(sockfd, &buf, sizeof(buf) - 1, 0, sa, &len);
        ((char *)&buf)[n] = 0;
        buf.sap_header = ntohl(buf.sap_header);
        printf("From %s hash 0x%04x\n", sock_ntop(sa, len), buf.sap_header & SAP_HASH_MASK);
        if (((buf.sap_header & SAP_VERSION_MASK) >> SAP_VERSION_SHIFT) > 1) {
            err_msg("... version field not 1 (0x%08x)", buf.sap_header);
            continue;
        }
        if (buf.sap_header & SAP_IPV6) {
            err_msg("... IPv6");
            continue;
        }
        if (buf.sap_header & (SAP_DELETE|SAP_ENCRYPTED|SAP_COMPRESSED)) {
            err_msg("... can't parse this packet type (0x%08x)", buf.sap_header);
            continue;
        }
        p = buf.sap_data + ((buf.sap_header & SAP_AUTHLEN_MASK) >> SAP_AUTHLEN_SHIFT);
        if (strcmp(p, "application/sdp") == 0) {
            p += 16;
        }
        printf("%s\n", p);
    }
}
