#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

#define IPADDR  "127.0.0.1"
#define PORT    8888
#define MAXLINE 4096
#define LISTENQ 5
#define SIZE    10

typedef struct server_context_s
{
    int     cli_cnt;        /* 客户端连接数，用于set */
    int     cli_fds[SIZE];  /* 连接客户端的 socket fd */
    fd_set  allfds;         /* 连接客户端的socket fd集 */
} srv_ctx_t;

static srv_ctx_t    *s_srv_ctx = NULL;

static int server_init(const char *ip, const int port, int &srv_fd);

int
main(int argc, char *argv[])
{
    int     srv_fd;

    if (server_init(IPADDR, PORT, &srv_fd) < 0) {
        fprintf(stderr, "server create fail.\n");
        exit(EXIT_FAILURE);
    }
    handle(srv_fd);
    exit(EXIT_SUCCESS);
}

/*
 * 以守护进程方式初始化服务端服务
 */
// 学完第十六章在回过来写
