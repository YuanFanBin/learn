#!/bin/bash

# 17.6
gcc prifinfo.c ../lib/error.c ../lib/sock_ntop_host.c get_ifi_info.c -o prifinfo
rm prifinfo
echo '[17.6 DONE]'
echo ''

# 17.8
gcc prmac.c ../lib/error.c ../lib/sock_ntop.c get_ifi_info.c -o prmac
rm prmac
echo '[17.8 DONE]'
echo ''
