#!/bin/bash

# 15.2
gcc unixbind.c ../lib/error.c -o unixbind
rm unixbind
echo '[15.2] DONE'
echo ''

# 15.5
gcc unixstrserv01.c ../lib/error.c ../Chapter05/str_echo.c ../Chapter05/sigchildwaitpid.c -o unixstrserv01
rm unixstrserv01
gcc unixstrcli01.c ../lib/error.c ../Chapter03/readline.c ../Chapter05/str_cli.c -o unixstrcli01
rm unixstrcli01
echo '[15.5] DONE'
echo ''

# 15.6
gcc unixdgserv01.c ../lib/error.c ../Chapter08/dg_echo.c -o unixdgserv01
rm unixdgserv01
gcc unixdgcli01.c ../lib/error.c ../Chapter08/dg_cli.c -o unixdgcli01
rm unixdgcli01
echo '[15.6] DONE'
echo ''

# 15.7
gcc mycat.c myopen.c read_fd.c ../lib/error.c -o mycat
rm mycat
gcc openfile.c write_fd.c ../lib/error.c -o openfile
rm openfile
echo '[15.7] DONE'
echo ''
