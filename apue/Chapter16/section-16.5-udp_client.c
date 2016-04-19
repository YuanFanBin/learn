 /* 参考资料：https://en.wikipedia.org/wiki/Berkeley_sockets */

/* Client-server example using UDP */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main(void)
{
    int                 sock;
    struct sockaddr_in  sa;
    int                 bytes_sent;
    char                buffer[200];

    strcpy(buffer, "hello world");

    /* create an Internet, datagram, socket using UDP */
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        /* if socket failed to initialize, exit */
        printf("Error Creating Socket");
        exit(EXIT_FAILURE);
    }

    /* Zero out socket address */
    memset(&sa, 0, sizeof(sa));

    /* The address is IPV4 */
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_port = htons(7654);

    bytes_sent = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&sa, sizeof(sa));
    if (bytes_sent < 0) {
        printf("Error sending packet: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    close(sock);    /* close the socket */
    return 0;
}
