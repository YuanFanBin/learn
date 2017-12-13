## 第五章 TCP客户/服务器程序示例

#### 5.1 概述

通过一个简单的样例，我们讨论一下客户服务器之间的关系，及各种可能出现的问题。

![简单的回射客户/服务器](doc/figure-5-1.png)

- 客户和服务器启动时发生了什么？

- 客户正常终止时发生了什么？

- 若服务器进程在客户之前终止，则客户会发生什么？

- 若服务器主机崩溃，则客户发生什么？

- 等。。。。

#### 5.2 TCP回射服务器程序：[main](tcpserv01.c) 函数

```c
#include <arpa/inet.h>      /* htonl, htons */
#include <errno.h>          /* errno */
#include <netinet/in.h>     /* sockaddr_in */
#include <stdio.h>
#include <stdlib.h>         /* exit */
#include <strings.h>        /* bzero */
#include <sys/socket.h>     /* socklen_t */
#include <unistd.h>         /* fork, read, write */
#include "../lib/error.h"

#define SERV_PORT   9877    /* TCP and UDP client-servers */
#define LISTENQ     1024    /* 2nd argument to listen() */
#define MAXLINE     4096    /* max text line length */

void str_echo(int sockfd);

int main(int argc, char **argv)
{
    int                 err;
    int                 listenfd, connfd;
    pid_t               childpid;
    socklen_t           clilen;
    struct sockaddr_in  cliaddr, servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if ((err = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
        err_sys("bind error");
    }

    if ((err = listen(listenfd, LISTENQ)) < 0) {
        err_sys("listen error");
    }

    for ( ; ; ) {
again:
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
#ifdef EPROTP
            if (errno == EPROTO || errno == ECONNABORTED) {
#else
            if (errno == ECONNABORTED) {
#endif
                goto again;
            } else {
                err_sys("accept error");
            }
        }
        if ((childpid = fork()) == -1) {
            err_sys("fork error");
        } else if (childpid == 0) {         /* child process */
            if (close(listenfd) == -1) {    /* close listening socket */
                err_sys("close error");
            }
            str_echo(connfd);               /* process the request */
            exit(0);
        }
        if (close(connfd) == -1) {
            err_sys("close error");
        }
    }
}
```

#### 5.3 TCP回射服务器程序：[str_echo](str_echo.c) 函数

```c
#include <unistd.h>     /* ssize_t, read, write */
#include <errno.h>      /* errno */
#include "../lib/error.h"

#define MAXLINE 4096    /* max text line length */

void str_echo(int sockfd)
{
    ssize_t n;
    char    buf[MAXLINE];
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        if (write(sockfd, buf, n) != n) {
            err_sys("write error");
        }
    }
    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        err_sys("str_echo: read error");
    }
}
```

结合 *5.2*, *5.3* 我们做一个小测试，查看基本功能。

```sh
# 服务端
$ gcc tcpserv01.c str_echo.c ../lib/error.c -o tcpserv01
$ ./tcpserv01

# 客户端
$ nc -vt localhost 9877
localhost [127.0.0.1] 9877 open
hello
hello

# 进程树状态
$ pstree
systemd─┬
        ...
        ├─st───zsh───tcpserv01───tcpserv01
        ...
```

#### 5.4 TCP回射客户程序：[main](tcpcli01.c) 函数

```c
#include <sys/socket.h>     /* socket */
#include <netinet/in.h>     /* sockaddr_in */
#include <arpa/inet.h>      /* inet_pton */
#include <strings.h>         /* bzero */
#include <errno.h>          /* errno */
#include <stdlib.h>         /* exit */
#include <stdio.h>
#include "../lib/error.h"

#define SERV_PORT   9877    /* TCP and UDP client-servers */
#define MAXLINE     4096    /* max text line length */

void str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv)
{
    int                 n;
    int                 sockfd;
    struct sockaddr_in  servaddr;

    if (argc != 2) {
        err_quit("usage: tcpcli <IPaddress>");
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if ((n = inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) < 0) {
        err_sys("inet_pton error for %s", argv[1]); /* errno set */
    } else if (n == 0) {
        err_quit("inet_pton error for %s", argv[1]);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        err_sys("connect error, errno = %d", errno);
    }

    str_cli(stdin, sockfd); /* do it all */
    exit(0);
}
```

#### 5.5 TCP回射客户程序：[str_cli](str_cli.c) 函数

```c
#include <stdio.h>      /* FILE */
#include <string.h>     /* strlen */
#include <unistd.h>     /* write */
#include "../lib/error.h"

#define MAXLINE     4096    /* max text line length */

// Chapter03/readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (write(sockfd, sendline, strlen(sendline)) != strlen(sendline)) {
            err_sys("write error");
        }
        if (readline(sockfd, recvline, MAXLINE) == 0) {
            err_quit("str_cli: serve terminate permaturely");
        }
        if (fputs(recvline, stdout) == EOF) {
            err_sys("fputs error");
        }
    }
}
```

结合 *5.2*, *5.3*, *5.4*, *5.5* 我们拥有了一对TCP客户服务程序，查看基本功能。

```sh
# 服务端
$ gcc tcpserv01.c str_echo.c ../lib/error.c -o tcpserv01
$ ./tcpserv01

# 客户端
$ gcc tcpcli01.c str_cli.c ../lib/error.c ../Chapter03/readline.c -o tcpcli01
$ ./tcpcli01 127.0.0.1
hello
hello
```

#### 5.6 正常启动

*socket* -> *bind* -> *listen* -> *accept*，并阻塞等待 *accept* 调用。

```sh
$ ./tcpserv01 &

# 查看端口占用情况
$ lsof -i
COMMAND     PID   USER   FD   TYPE   DEVICE SIZE/OFF NODE NAME
tcpserv01  3159 fanbin    3u  IPv4 52420503      0t0  TCP *:9877 (LISTEN)
...

$ ./tcpcli01 127.0.0.1
```

客户调用 *socket* 和 *connect* （此调用引起三路握手操作），当三路握手完成后，连接建立。

- 客户阻塞至 *str_cli* 函数的 *fgets* 调用。

- 服务器阻塞至 *accept*，当有连接时，父进程 *fork* 出子进程，让子进程回射客户数据

- 父进程继续提供服务，阻塞在 *accept* 上。

#### 5.7 正常终止

- 键入 *EOF* 字符，按客户流程 *main* 调用 *exit* 终止（*exit* 调用会关闭所有已打开文件描述服，其中的 *socket* 连接断开）。客户TCP发送 *FIN* 给服务器，服务器TCP则以 *ACK* 响应，这是TCP连接终止序列的前半部分。此时服务器套接字处于 *CLOSE_WAIT* 状态，客户套接字则处于 *FIN_WAIT_2_状态

出现的问题
1. 僵死子进程：通过捕获SIGCHLD信号并使用waitpid来处理；
2. 服务器进程终止，客户进程没有被告知：第六章 select，poll函数处理；
3. 服务器主机崩溃，客户端发送了数据才能检测：第七章 SO_KEEPALIVE 套接字选项来解决；
4. 发送数值数据大小端问题。

图片来源：[UNP](https://www.amazon.cn/UNIX%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B-%E5%A5%97%E6%8E%A5%E5%AD%97%E8%81%94%E7%BD%91API-%E5%8F%B2%E8%92%82%E6%96%87%E6%96%AF/dp/B011S72JB6/ref=sr_1_3?ie=UTF8&qid=1512463174&sr=8-3&keywords=unix+network+programming)
