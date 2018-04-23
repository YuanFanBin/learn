#!/bin/bash

# 25.3
########################################
gcc ../Chapter08/udpcli01.c ../lib/error.c ../Chapter08/dg_cli.c -o udpcli01
gcc ../Chapter08/udpserv01.c ../lib/error.c dgecho01.c -o udpserv01
rm udpcli01 udpserv01
echo '[25.3 DONE]'
echo ''
########################################
