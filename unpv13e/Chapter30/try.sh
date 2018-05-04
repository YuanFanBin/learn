#!/bin/bash

########################################
# 30.3
gcc client.c ../lib/error.c ../Chapter03/readn.c ../Chapter11/tcp_connect.c -o client
rm client
echo '[30.3 DONE]'
echo ''
########################################

########################################
# 30.4
gcc serv00.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter26/readline.c pr_cpu_time.c web_child.c -lpthread -o serv00
rm serv00
echo '[30.4 DONE]'
echo ''
########################################

########################################
# 30.5
gcc serv01.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter26/readline.c pr_cpu_time.c web_child.c sig_chld_waitpid.c -lpthread -o serv01
rm serv01
echo '[30.5 DONE]'
echo ''
########################################

########################################
# 30.6
gcc serv02.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter26/readline.c pr_cpu_time.c web_child.c child02.c -lpthread -o serv02
rm serv02
echo '[30.6 DONE]'
echo ''
########################################

########################################
# 30.6.3
gcc serv02m.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter26/readline.c pr_cpu_time.c web_child.c child02m.c meter.c -lpthread -o serv02m
rm serv02m
echo '[30.6.3 DONE]'
echo ''
########################################

########################################
# 30.7
gcc serv03.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter26/readline.c pr_cpu_time.c web_child.c child03.c lock_fcntl.c -lpthread -o serv03
rm serv03
echo '[30.7 DONE]'
echo ''
########################################

########################################
# 30.8
gcc serv03.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter26/readline.c pr_cpu_time.c web_child.c child03.c lock_pthread.c -lpthread -o serv03
rm serv03
echo '[30.8 DONE]'
echo ''
########################################

########################################
# 30.9
gcc serv05.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter15/read_fd.c ../Chapter15/write_fd.c ../Chapter26/readline.c pr_cpu_time.c web_child.c child05.c -lpthread -o serv05
rm serv05
echo '[30.9 DONE]'
echo ''
########################################

########################################
# 30.10
gcc serv06.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter26/readline.c pr_cpu_time.c web_child.c -lpthread -o serv06
rm serv06
echo '[30.10 DONE]'
echo ''
########################################

########################################
# 30.11
gcc serv07.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter26/readline.c pr_cpu_time.c web_child.c pthread07.c -lpthread -o serv07
rm serv07
echo '[30.11 DONE]'
echo ''
########################################

########################################
# 30.12
gcc serv08.c ../lib/error.c ../Chapter03/writen.c ../Chapter11/tcp_listen.c ../Chapter26/readline.c pr_cpu_time.c web_child.c pthread08.c -lpthread -o serv08
rm serv08
echo '[30.12 DONE]'
echo ''
########################################
