#!/bin/bash

# 5.2 & 5.3
gcc tcpserv01.c str_echo.c ../lib/error.c -o tcpserv01
rm tcpserv01
echo '[5.2 & 5.3] DONE'
echo ''

# 5.4 & 5.5
gcc tcpcli01.c str_cli.c ../lib/error.c ../Chapter03/readline.c -o tcpcli01
rm tcpcli01
echo '[5.4 & 5.5] DONE'
echo ''
