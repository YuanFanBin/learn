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
 * gcc tcp_server.c -o server 
 * $ ./server
 * */
int main(int argc, char *argv[])
{
    int    srv_sockfd;
    int    cli_sockfd;
    size_t len;
    size_t sin_size;
    struct sockaddr_in srv_addr;
    struct sockaddr_in cli_addr;
    char   buf[BUFSIZE];

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &srv_addr.sin_addr);
    srv_addr.sin_port = htons(PORT);

    /* TCP: socket套接字 */
    if ((srv_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "create socket fail, %s.\n", strerror(errno));
        return 1;
    }

    /* bind地址 */
    if (bind(srv_sockfd, (struct sockaddr*)&srv_addr, 
        sizeof(struct sockaddr)) < 0) 
    {
        fprintf(stderr, "create bind fail, %s.\n", strerror(errno));
        return 1;
    }

    /* listen接口 */
    listen(srv_sockfd, LISTENQ);

    sin_size = sizeof(struct sockaddr_in);
    /* 等待客户端连接请求 */
    if ((cli_sockfd = accept(srv_sockfd, (struct sockaddr*)&cli_addr, 
        &sin_size)) < 0)
    {
        fprintf(stderr, "create accept fail, %s.\n", strerror(errno));
        return 1;
    }

    fprintf(stdout, "accept client %s\n", inet_ntoa(cli_addr.sin_addr));
    sprintf(buf, "Hello client, ip:%s\n", inet_ntoa(cli_addr.sin_addr));
    len = send(cli_sockfd, buf, strlen(buf), 0);

    /* 接受客户端数据 */
    memset(&buf, 0, sizeof(char) * BUFSIZE);
    while((len = recv(cli_sockfd, buf, BUFSIZE, 0)) > 0) {
        buf[len] = '\0';
        fprintf(stdout, "%s\n", buf);
        if (send(cli_sockfd, buf, len, 0) < 0) { /* 原样发回 */
            fprintf(stderr, "create accept fail, %s.\n", strerror(errno));
            return 1;
        }
    }
    close(cli_sockfd);
    close(srv_sockfd);
    return 0;
}
