#include "udpcksum.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// define gloabl variables
struct sockaddr     *dest, *local;
struct sockaddr_in   locallookup;
socklen_t            destlen, locallen;

int     datalink;      // from pcap_datalink(), in <net/bpf.h>
char   *device;        // pcap device
pcap_t *pd;            // packet capture struct pointer
int     rawfd;         // raw socket to write on
int     snaplen = 200; // amount of data to capture
int     verbose;
int     zerosum;       // send UDP query with no checksum

static void usage(const char *msg);

// ../../Chapter11/host_serv.c
struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype);

int main(int argc, char *argv[1])
{
    int             c, s, lopt = 0;
    char            *ptr, localname[1024], *localport;
    struct addrinfo *aip;

    opterr = 0; // don't want getopt() writing to stderr
    while ((c = getopt(argc, argv, "0i:l:v")) != -1) {
        switch (c) {
            case '0':
                zerosum = 1;
                break;

            // 用于指定接收服务器的应答的接口
            case 'i':
                device = optarg;    // pcap device
                break;

            // 用于指定源IP地址和源端口号
            case 'l':   // local IP address and port #: a.b.c.d.p
                if ((ptr = strrchr(optarg, '.')) == NULL) {
                    usage("invalid -l option");
                }
                *ptr++ = 0;         // null replaces final period
                localport = ptr;    // service name or port number
                strncpy(localname, optarg, sizeof(localname));
                lopt = 1;
                break;

            case 'v':
                verbose = 1;
                break;

            case '?':
                usage("unrecognized option");
        }
    }

    if (optind != argc - 2) {
        usage("missing <host> and/or <serv>");
    }

    // 验证剩余命令行参数恰好是两个：运行DNS服务器的目的主机或IP地址
    // 以及服务器的服务名或端口号。
    // convert destination name and service
    aip = host_serv(argv[optind], argv[optind + 1], AF_INET, SOCK_DGRAM);
    dest = aip->ai_addr;    // don't freeaddrinfo()
    destlen = aip->ai_addrlen;

    if (lopt) {
        // 如果本地主机名（或IP地址）和端口号已经作为命令行 -l 选项的参数
        // 指定，那就对它们执行同样的转换，并把转换的套接字地址结构的指针
        // 存入local。
        aip = host_serv(localname, localport, AF_INET, SOCK_DGRAM);
        local = aip->ai_addr;   // don't freeaddrinfo()
        locallen = aip->ai_addrlen;
    } else {
        // 否则我们通过把一个UDP套接字连接到目的地确定由内核选定的本地IP
        // 地址和临时端口号。
        if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            err_sys("socket error");
        }
        if (connect(s, dest, destlen) < 0) {
            err_sys("connect error");
        }
        // kernel chooses correct local address for dest
        locallen = sizeof(locallookup);
        local = (struct sockaddr *) &locallookup;
        if (getsockname(s, local, &locallen) < 0) {
            err_sys("getsockname error");
        }
        if (locallookup.sin_addr.s_addr == htonl(INADDR_ANY)) {
            err_quit("Can't determine local address - use -l\n");
        }
        close(s);
    }

    // 调用open_output函数创建一个原始套接字并开启 IP_HDRINCL 套接字选项
    // 于是可以往这个套接字写出包括IP首部在内的完整IP数据报
    open_output();  // open output, ether raw socket or libnet

    // 打开分组捕获设备
    open_pcap();    // open packet capture device

    setuid(getuid());   // don't need superuser privileges anymore

    signal(SIGTERM, cleanup);
    signal(SIGINT, cleanup);
    signal(SIGHUP, cleanup);

    test_udp();

    cleanup(0);
}

static void usage(const char *msg)
{
    err_msg(
"usage: udpcksum [ options  ] <host> <serv>\n"
"options: -0    send UDP datagram with checksum set to 0\n"
"         -i s  packet capture device\n"
"         -l a.b.c.d.p  local IP=a.b.c.d, local port=p\n"
"         -v    verbose output");
    if (msg[0] != 0) {
        err_quit("%s", msg);
    }
    exit(1);
}
