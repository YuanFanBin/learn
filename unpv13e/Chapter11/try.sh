#!/bin/bash

# 11.3
gcc hostent.c ../lib/error.h ../lib/error.c -o hostent
rm hostent
echo '[11.3] DONE'
echo ''


## 11.5
gcc daytimetcpcli1.c ../lib/error.h ../lib/error.c ../lib/sock_ntop.c -o daytimetcpcli1
rm daytimetcpcli1
echo '[11.5] DONE'
echo ''

## 11.6
gcc getaddrinfo_srv.c -o srv
rm srv
gcc getaddrinfo_cli.c -o cli
rm cli
echo '[11.6] DONE'
echo ''

## 11.12
gcc daytimetcpcli.c tcp_connect.c -o daytimetcpcli
rm daytimetcpcli
echo '[11.12] DONE'
echo ''

## 11.13.1
gcc daytimetcpsrv1.c tcp_listen.c ../lib/sock_ntop.c -o daytimetcpsrv1
rm daytimetcpsrv1
echo '[11.13.1] DONE'
echo ''

## 11.13.2
gcc daytimetcpsrv2.c tcp_listen.c ../lib/sock_ntop.c -o daytimetcpsrv2
rm daytimetcpsrv2
echo '[11.13.2] DONE'
echo ''

## 11.14
gcc daytimeudpcli1.c udp_client.c ../lib/sock_ntop_host.c -o daytimeudpcli1
rm daytimeudpcli1
echo '[11.14] DONE'
echo ''

### 11.16
gcc daytimeudpsrv2.c udp_server.c ../lib/sock_ntop.c -o daytimeudpsrv2
rm daytimeudpsrv2
echo '[11.16] DONE'
echo ''
