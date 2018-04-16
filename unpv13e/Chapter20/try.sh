#!/bin/bash

# 20.4
sed -i 's/htons(SERV_PORT)/htons(13)/g' ../Chapter08/udpcli01.c
gcc ../Chapter08/udpcli01.c ../lib/error.c ../Chapter08/dg_cli.c -o udpcli
rm udpcli

# dgclibcast1
gcc ../Chapter08/udpcli01.c ../lib/error.c ../lib/sock_ntop_host.c dgclibcast1.c -o udpcli
rm udpcli

# dgclibcast3
gcc ../Chapter08/udpcli01.c ../lib/error.c ../lib/sock_ntop_host.c dgclibcast3.c -o udpcli
rm udpcli

# dgclibcast4
gcc ../Chapter08/udpcli01.c ../lib/error.c ../lib/sock_ntop_host.c dgclibcast4.c -o udpcli
rm udpcli

sed -i 's/htons(13)/htons(SERV_PORT)/g' ../Chapter08/udpcli01.c

echo '[20.4 DONE]'
echo ''
