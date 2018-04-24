#!/bin/bash

# 26.4
########################################
gcc tcpserv01.c ../lib/error.c ../Chapter05/str_echo.c ../Chapter11/tcp_listen.c -lpthread -o tcpserv01
gcc tcpserv02.c ../lib/error.c ../Chapter05/str_echo.c ../Chapter11/tcp_listen.c -lpthread -o tcpserv02
gcc ../Chapter05/tcpcli01.c ../lib/error.c strclithread.c ../Chapter03/readline.c ../Chapter03/writen.c -lpthread -o tcpcli01
rm tcpserv01 tcpserv02 tcpcli01
echo '[26.4 DONE]'
echo ''
########################################

# 26.7
########################################
gcc example01.c ../lib/error.c -lpthread -o example01
gcc example02.c ../lib/error.c -lpthread -o example02
rm example01 example02
echo '[26.7 DONE]'
echo ''
########################################

# 26.9
########################################
gcc web03.c ../Chapter16/home_page.c ../Chapter16/write_get_cmd.c ../Chapter11/tcp_connect.c ../Chapter03/writen.c ../lib/error.c -lpthread -o web03
rm web03
echo '[26.9 DONE]'
echo ''
########################################
