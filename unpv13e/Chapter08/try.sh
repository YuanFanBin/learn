#!/bin/bash

# 8.3(udpserv01.c #1) & 8.4
gcc udpserv01.c ../lib/error.c dg_echo.c -o udpsrv
rm udpsrv
echo '[8.3(udpserv01.c #1) & 8.4] DONE'
echo ''

# 8.5(udpcli01.c #1) & 8.6
gcc udpcli01.c ../lib/error.c dg_cli.c -o udpcli
rm udpcli
echo '[8.5(udpcli01.c #1) & 8.6] DONE'
echo ''

# 8.8(udpcli01.c #2)
gcc udpcli01.c ../lib/error.c dg_cliaddr.c ../lib/sock_ntop.c -o udpcli
rm udpcli
echo '[8.8(udpcli01.c #2)] DONE'
echo ''

# 8.12(udpcli01.c #3)
gcc udpcli01.c ../lib/error.c dg_cliconnect.c -o udpcli
rm udpcli
echo '[8.12(udpcli01.c #3)] DONE'
echo ''

# 8.13(udpcli01.c #4, udpserv01.c #2, #3)
gcc udpcli01.c ../lib/error.c dg_cliloop1.c -o udpcli
rm udpcli
gcc udpserv01.c ../lib/error.c dg_echoloop1.c -o udpsrv
rm udpsrv
gcc udpserv01.c ../lib/error.c dg_echoloop2.c -o udpsrv
rm udpsrv
echo '[8.13(udpcli01.c #4, udpserv01.c #2, #3)] DONE'
echo ''

# 8.14
gcc udpserv01.c ../lib/error.c dg_echoloop2.c -o udpsrv
rm udpsrv
echo '[8.14] DONE'
echo ''

# 8.15
gcc udpserv_select01.c ../lib/error.c ../Chapter05/str_echo09.c ../Chapter05/sigchildwaitpid.c -o udpsrv
rm udpsrv
echo '[8.15] DONE'
echo ''
