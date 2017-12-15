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

# 5.9
gcc tcpserv01.c str_echo.c ../lib/error.c sigchildwait.c -o tcpserv02
rm tcpserv02
echo '[5.9] DONE'
echo ''

# 5.10
gcc tcpcli04.c str_cli.c ../lib/error.c ../Chapter03/readline.c -o tcpcli04
rm tcpcli04
gcc tcpserv04.c str_echo.c sigchildwaitpid.c ../lib/error.c -o tcpserv04
rm tcpserv04
echo '[5.10] DONE'
echo ''


# 5.13
gcc tcpcli01.c str_cli11.c ../lib/error.c ../Chapter03/readline.c -o tcpcli11
rm tcpcli11
echo '[5.13] DONE'
echo ''
