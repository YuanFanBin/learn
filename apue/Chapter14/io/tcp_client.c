#include <stdio.h>      /* stdin, stdout, stderr */
#include <string.h>     /* memset, strerror */
#include <sys/types.h>  /* socket, bind, listen, send */
#include <sys/socket.h> /* socket, bind, listen, send */
#include <netinet/in.h> /* sin_family, sin_port, sin_addr (man in.h) */
#include <arpa/inet.h>  /* inet_pton, htons (man inet.h) */
#include <errno.h>      /* errno */

#define BUFSIZE 1024
#define IP      "127.0.0.1"
#define PORT    8888
#define LISTENQ 5

/* 
 * gcc tcp_client.c -o client 
 * $ ./client
 * */
int main(int argc, char *argv[])
{
    int    cli_sockfd;
    size_t len;
    struct sockaddr_in srv_addr;
    char   buf[BUFSIZE];

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &srv_addr.sin_addr);
    srv_addr.sin_port = htons(PORT);

    /* TCP: socket套接字 */
    if ((cli_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "create socket fail, %s.\n", strerror(errno));
        return 1;
    }

    /* connect server */
    if (connect(cli_sockfd, (struct sockaddr*)&srv_addr, 
        sizeof(struct sockaddr)) < 0)
    {
        fprintf(stderr, "create connect fail, %s.\n", strerror(errno));
        return 1;
    }

    fprintf(stdout, "connected to server\n");
    len = recv(cli_sockfd, buf, BUFSIZE, 0);
    buf[len] = '\0';
    fprintf(stdout, "%s\n", buf);

    while(1) {
        fprintf(stdout, "Enter string to send:");
        scanf("%s", buf);
        len = send(cli_sockfd, buf, strlen(buf), 0);
        len = recv(cli_sockfd, buf, BUFSIZE, 0);
        buf[len] = '\0';
        fprintf(stdout, "received:%s\n", buf);
    }
    close(cli_sockfd);
    return 0;
}
