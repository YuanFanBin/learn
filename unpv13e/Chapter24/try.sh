#!/bin/bash

# 24.2.1
########################################
gcc tcpsend01.c ../lib/error.c ../Chapter11/tcp_connect.c -o tcpsend01
gcc tcprecv01.c ../lib/error.c ../Chapter11/tcp_listen.c -o tcprecv01
rm tcpsend01 tcprecv01
echo '[24.2.1 DONE]'
echo ''
########################################

# 24.2.2
########################################
gcc tcpsend01.c ../lib/error.c ../Chapter11/tcp_connect.c -o tcpsend01
gcc tcprecv02.c ../lib/error.c ../Chapter11/tcp_listen.c -o tcprecv02
gcc tcprecv03.c ../lib/error.c ../Chapter11/tcp_listen.c -o tcprecv03
rm tcpsend01 tcprecv02 tcprecv03
echo '[24.2.2 DONE]'
echo ''
########################################

# 24.3.1
########################################
gcc tcpsend04.c ../lib/error.c ../Chapter11/tcp_connect.c -o tcpsend04
gcc tcprecv04.c ../lib/error.c ../Chapter11/tcp_listen.c -o tcprecv04
rm tcpsend04 tcprecv04
echo '[24.3.1 DONE]'
echo ''
########################################

# 24.3.2
########################################
gcc tcpsend05.c ../lib/error.c ../Chapter11/tcp_connect.c -o tcpsend05
gcc tcprecv05.c ../lib/error.c ../Chapter11/tcp_listen.c -o tcprecv05
rm tcpsend05 tcprecv05
echo '[24.3.2 DONE]'
echo ''
########################################

# 24.3.3
########################################
gcc tcpsend06.c ../lib/error.c ../Chapter11/tcp_connect.c -o tcpsend06
gcc tcprecv04.c ../lib/error.c ../Chapter11/tcp_listen.c -o tcprecv04
rm tcpsend06 tcprecv04
echo '[24.3.3 DONE]'
echo ''
########################################

# 24.5
########################################
gcc ../Chapter05/tcpcli01.c ../lib/error.c strcliselect02.c ../Chapter03/writen.c ../Chapter03/readline.c heartbeatcli.c -o tcpcli01
gcc ../Chapter05/tcpserv04.c ../lib/error.c ../Chapter05/sigchildwaitpid.c strecho.c ../Chapter03/writen.c ../Chapter03/readline.c heartbeatserv.c -o tcpserv04
rm tcpcli01 tcpserv04
echo '[24.5 DONE]'
echo ''
########################################
