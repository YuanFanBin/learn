#include "udpcksum.h"
#include <arpa/inet.h>
#include <stdio.h>

#define CMD "udp and src host %s and src port %d"

// ../../lib/sock_ntop_host.c
char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
// ../../Chapter28/icmpd/sock_get_port.c
int sock_get_port(const struct sockaddr *sa, socklen_t salen);

// 打开分组捕获设备
void open_pcap(void)
{
    uint32_t localnet, netmask;
    char cmd[MAXLINE], errbuf[PCAP_ERRBUF_SIZE];
    char str1[INET_ADDRSTRLEN], str2[INET_ADDRSTRLEN];
    struct bpf_program fcode;

    // 选择分组设备：如果分组设备未曾指定（通过 -i 选项），那就调用
    // pcap_lookupdev函数选择一个设备
    if (device == NULL) {
        if ((device = pcap_lookupdev(errbuf)) == NULL) {
            err_quit("pcap_lookup: %s", errbuf);
        }
    }
    printf("device = %s\n", device);

    // 打开设备
    // hardcode: promisc = 0, to_ms = 500
    if ((pd = pcap_open_live(device, snaplen, 0, 500, errbuf)) == NULL) {
        err_quit("pcap_open_live: %s", errbuf);
    }

    // 获取网络地址与子网掩码
    if (pcap_lookupnet(device, &localnet, &netmask, errbuf) < 0) {
        err_quit("pcap_lookupnet: % s", errbuf);
    }
    if (verbose) {
        printf("localnet = %s, netmask = %s\n",
               inet_ntop(AF_INET, &localnet, str1, sizeof(str1)),
               inet_ntop(AF_INET, &netmask, str2, sizeof(str2)));
    }

    // 编译分组过滤器
    snprintf(cmd, sizeof(cmd), CMD, sock_ntop_host(dest, destlen), ntohs(sock_get_port(dest, destlen)));
    if (verbose) {
        printf("cmd = %s\n", cmd);
    }
    if (pcap_compile(pd, &fcode, cmd, 0, netmask) < 0) {
        err_quit("pcap_compile: %s", pcap_geterr(pd));
    }

    // 装载过滤程序
    if (pcap_setfilter(pd, &fcode) < 0) {
        err_quit("pcap_setfilter: %s", pcap_geterr(pd));
    }

    // 确定数据链路类型
    if ((datalink = pcap_datalink(pd)) < 0) {
        err_quit("pcap_datalink: %s", pcap_geterr(pd));
    }
    if (verbose) {
        printf("datalink = %d\n", datalink);
    }
}

// 获取来自分组捕获设备的下一个分组
char *next_pcap(int *len)
{
    char               *ptr;
    struct pcap_pkthdr  hdr;
    // struct pcap_pkthdr {
    //     struct timeval ts;      // timestamp
    //     bpf_u_int32    caplen;  // length of portion captured
    //     bpf_u_int32    len;     // length of this packet (off wire)
    // };

    // keep looping util packet ready
    while ((ptr = (char *) pcap_next(pd, &hdr)) == NULL) { }

    *len = hdr.caplen;  // captured length
    return(ptr);
}
