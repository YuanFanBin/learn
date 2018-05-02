#include "udpcksum.h"
#include <strings.h>

int rawfd;  // raw socket to write on

void open_output(void)
{
    int on = 1;
    /*
     * Need a raw socket to write our own IP datagrams to.
     * Process must have superuser privileges to create this
     * socket. Also must set IP_HDRINCL so we can write our
     * own IP headers
     */
    if ((rawfd = socket(dest->sa_family, SOCK_RAW, 0)) < 0) {
        err_sys("socket error");
    }
    if (setsockopt(rawfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
        err_sys("setsockopt error");
    }
}

// ../../Chapter28/ping/in_cksum.c
uint16_t in_cksum(uint16_t *addr, int len);

// 构造UDP首部和IP首部，往原始套接字写出IP数据报
void udp_write(char *buf, int userlen)
{
    struct udpiphdr *ui;
    struct ip       *ip;

    // UDP首部
    // fill in and checksum UDP header
    ip = (struct ip *) buf;
    ui = (struct udpiphdr *) buf;
    bzero(ui, sizeof(*ui));
    // add 8 to userlen for pseudoheader length
    ui->ui_len = htons((uint16_t) (sizeof(struct udphdr) + userlen));
    // then and 28 for IP datagram length
    userlen += sizeof(struct udpiphdr);

    ui->ui_pr = IPPROTO_UDP;
    ui->ui_src.s_addr = ((struct sockaddr_in *) local)->sin_addr.s_addr;
    ui->ui_dst.s_addr = ((struct sockaddr_in *) dest)->sin_addr.s_addr;
    ui->ui_sport = ((struct sockaddr_in *) local)->sin_port;
    ui->ui_dport = ((struct sockaddr_in *) dest)->sin_port;
    ui->ui_ulen = ui->ui_len;
    if (zerosum == 0) {
#if 1   // change to if 0 for Solaris 2.x x < 6
        if ((ui->ui_sum = in_cksum((u_int16_t *) ui, userlen)) == 0) {
            ui->ui_sum = 0xffff;
        }
#else
        ui->ui_sum = ui->ui_len;
#endif
    }

    // IP首部
    // fill in rset of IP header
    // ip_output() calcuates & stores IP header checksum
    ip->ip_v = IPVERSION;
    ip->ip_hl = sizeof(struct ip) >> 2;
    ip->ip_tos = 0;
#if defined(linux) || defined(__OpenBSD__)
    ip->ip_len = htons(userlen);    // network byte order
#else
    ip->ip_len = userlen;   // host byte order
#endif
    ip->ip_id = 0;      // let IP set this
    ip->ip_off = 0;     // frag offset, MF and DF flags
    ip->ip_ttl = TTL_OUT;

    sendto(rawfd, buf, userlen, 0, dest, destlen);
}
