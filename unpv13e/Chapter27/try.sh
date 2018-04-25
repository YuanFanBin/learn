#!/bin/bash

# 27.3.1
########################################
gcc tcpserv01.c ../Chapter05/str_echo.c ../Chapter11/host_serv.c ../lib/error.c sourceroute.c -o tcpserv01
gcc tcpcli01.c ../Chapter05/str_cli.c ../Chapter11/host_serv.c ../Chapter26/readline.c ../lib/error.c sourceroute.c -lpthread -o tcpcli01
rm tcpserv01 tcpcli01
echo '[27.3.1 DONE]'
echo ''
########################################
