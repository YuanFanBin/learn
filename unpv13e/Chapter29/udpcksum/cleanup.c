#include "udpcksum.h"
#include <stdio.h>
#include <stdlib.h>

void cleanup(int signo)
{
    struct pcap_stat stat;

    if (verbose) {
        if (pcap_stats(pd, &stat) < 0) {
            err_quit("pcap_stat: %s\n", pcap_geterr(pd));
        }
        printf("%d packets received by filter\n", stat.ps_recv);
        printf("%d packets dropped by kernel\n", stat.ps_drop);
    }

    exit(0);
}
