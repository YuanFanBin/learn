#!/bin/bash

# 1.2
gcc daytimetcpcli.c ../lib/error.c -o daytimetcpcli
rm daytimetcpcli
echo '[1.2] DONE'
echo ''

# 1.3
gcc daytimetcpcli6.c ../lib/error.c -o daytimetcpcli6
rm daytimetcpcli6
echo '[1.3] DONE'
echo ''

# 1.5
gcc daytimetcpsrv.c ../lib/error.c -o daytimetcpsrv
rm daytimetcpsrv
echo '[1.5] DONE'
echo ''
