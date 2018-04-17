#!/bin/bash

# 21.8
sed -i 's/htons(SERV_PORT)/htons(7)/' ../Chapter08/udpcli01.c
sed -i 's/setsockopt(.*)/0)/' ../Chapter20/dgclibcast1.c

gcc ../Chapter08/udpcli01.c ../lib/error.c ../lib/sock_ntop_host.c ../Chapter20/dgclibcast1.c -o udpcli

sed -i 's/(0)/(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, \&on, sizeof(on)) < 0)/' ../Chapter20/dgclibcast1.c
sed -i '/#3/s/htons(7)/htons(SERV_PORT)/' ../Chapter08/udpcli01.c

rm udpcli
echo '[21.8 DONE]'
echo ''

# 21.9
gcc mysdr/main.c mysdr/loop.c ../lib/error.c ../lib/sock_ntop.c ../Chapter11/udp_client.c mcast_join.c mcast.c -o sdr
rm sdr
echo '[21.9 DONE]'
echo ''

# 21.10
gcc mcast/main.c mcast/send.c mcast/recv.c ../lib/error.c ../lib/sock_ntop.c ../Chapter11/udp_client.c mcast_join.c mcast_set_loop.c mcast.c -o sendrecv
rm sendrecv
echo '[21.10 DONE]'
echo ''

# 21.11
gcc ssntp/main.c ssntp/sntp_proc.c ssntp/sock_set_wild.c ../lib/error.c ../lib/sock_ntop.c ../Chapter11/udp_client.c ../Chapter17/get_ifi_info.c mcast.c mcast_join.c -o sntp
rm sntp
echo '[21.11 DONE]'
echo ''
