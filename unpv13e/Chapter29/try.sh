#!/bin/bash


########################################
# 29.7
gcc udpcksum/main.c udpcksum/cleanup.c udpcksum/pcap.c udpcksum/senddnsquery-raw.c udpcksum/udpcksum.c udpcksum/udpread.c udpcksum/udpwrite.c ../Chapter11/host_serv.c ../lib/sock_ntop_host.c ../lib/error.c ../Chapter28/icmpd/sock_get_port.c ../Chapter28/ping/in_cksum.c -o udpcksum0
rm udpcksum0
echo '[29.7 DONE]'
echo 'struct udpiphdr{} => <netinet/ip_var.h> <netinet/udp_var.h> => FreeBSD'
echo ''
########################################
