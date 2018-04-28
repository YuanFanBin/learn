#!/bin/bash

# 28.5
########################################
# ping v4
gcc ping/main.c ../Chapter11/host_serv.c ../lib/error.c ../lib/sock_ntop_host.c ping/readloop.c ping/sig_alrm.c ping/tv_sub.c ping/send_v4.c ping/proc_v4.c ping/in_cksum.c -o pingv4
# ping v46
gcc -DIPV6 ping/main.c ../Chapter11/host_serv.c ../lib/error.c ../lib/sock_ntop_host.c ping/readloop.c ping/sig_alrm.c ping/tv_sub.c ping/send_v4.c ping/proc_v4.c ping/in_cksum.c ping/init_v6.c ping/send_v6.c ping/proc_v6.c -o pingv46
rm pingv4 pingv46
echo '[28.5 DONE]'
echo ''
########################################

# 28.6
########################################
# traceroute v4
gcc traceroute/main.c ../Chapter11/host_serv.c ../lib/sock_ntop_host.c ../lib/error.c traceroute/traceloop.c traceroute/sig_alrm.c traceroute/icmpcode_v4.c traceroute/recv_v4.c traceroute/sock_set_port.c traceroute/sock_cmp_addr.c ping/tv_sub.c -o traceroutev4
# traceroute v46
gcc -DIPV6 traceroute/main.c ../Chapter11/host_serv.c ../lib/sock_ntop_host.c ../lib/error.c traceroute/traceloop.c traceroute/sig_alrm.c traceroute/icmpcode_v4.c traceroute/recv_v4.c traceroute/sock_set_port.c traceroute/sock_cmp_addr.c ping/tv_sub.c traceroute/icmpcode_v6.c traceroute/recv_v6.c -o traceroutev46
rm traceroutev4 traceroutev46
echo '[28.6 DONE]'
echo ''
########################################


# 28.7.1
########################################
gcc -DIPV6 ../Chapter08/udpcli01.c icmpd/dgcli01.c icmpd/sock_bind_wild.c ../Chapter15/write_fd.c ../lib/error.c ../lib/sock_ntop.c -o udpcli01
rm udpcli01
echo '[28.7.1 DONE]'
echo ''
########################################

# 28.7.3
########################################
gcc -DIPV6 icmpd/icmpd.c icmpd/readable_conn.c icmpd/readable_listen.c icmpd/readable_v4.c icmpd/readable_v6.c icmpd/sock_bind_wild.c icmpd/sock_get_port.c ../lib/error.c ../Chapter15/read_fd.c ../lib/sock_ntop_host.c -o icmpdv46
rm icmpdv46
echo '[28.7.3 DONE]'
echo ''
########################################
