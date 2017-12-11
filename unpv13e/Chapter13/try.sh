#!/bin/bash

# 13.4
gcc daytimetcpsrv2.c daemon_init.c ../Chapter11/tcp_listen.c ../lib/sock_ntop.c ../lib/error.c -o daytimetcpsrv2
rm daytimetcpsrv2
echo '[13.4] DONE'
echo ''

# 13.6
gcc daytimetcpsrv3.c daemon_inetd.c ../lib/sock_ntop.c ../lib/error.c -o daytimetcpsrv3
rm daytimetcpsrv3
echo '[13.6] DONE'
echo ''
