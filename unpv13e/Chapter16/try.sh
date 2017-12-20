#!/bin/bash

# 16.5
gcc web.c home_page.c ../Chapter11/tcp_connect.c  ../Chapter03/writen.c start_connect.c ../Chapter11/host_serv.c write_get_cmd.c ../lib/error.c -o web
rm web
echo '[16.5 DONE]'
echo ''
