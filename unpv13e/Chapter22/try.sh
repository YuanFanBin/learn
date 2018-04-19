#!/bin/bash

# 22.2
########################################
gcc ../Chapter08/udpserv01.c ../lib/error.c ../lib/sock_ntop.c recvfromflags.c dgechoaddr.c -o udpsrv
rm udpsrv

# 8.5(udpcli01.c #1) & 8.6
gcc ../Chapter08/udpcli01.c ../lib/error.c ../Chapter08/dg_cli.c -o udpcli
rm udpcli

echo '[22.2 DONE]'
echo ''
########################################

# 22.5
########################################
# 8.3(udpserv01.c #1) & 8.4
gcc ../Chapter08/udpserv01.c ../lib/error.c ../Chapter08/dg_echo.c -o udpsrv
rm udpsrv

# 8.5(udpcli01.c #1) & 8.6
gcc ../Chapter08/udpcli01.c ../lib/error.c ./rtt/dg_send_recv.c ./rtt/rtt.c ./rtt/dg_cli.c -o udpcli
rm udpcli

echo '[22.5 DONE]'
echo ''
########################################

# 22.6
gcc udpserv03.c ../lib/error.c ../lib/sock_ntop.c ../Chapter17/get_ifi_info.c -o udpserv03
rm udpserv03
echo '[22.6 DONE]'
echo ''
