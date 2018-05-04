#include "web.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define min(a, b)   ((a) < (b) ? (a) : (b))

int main(int argc, char **argv)
{
    int i, fd, n, maxnconn, flags, error;
    char buf[MAXLINE];
    fd_set rs, ws;

    if (argc < 5) {
        err_quit("usage: web <#conns> <hostname> <homepage> <file> ...");
    }
    maxnconn = atoi(argv[1]);

    nfiles = min(argc - 4, MAXFILES);
    for (i = 0; i < nfiles; i++) {
        file[i].f_name = argv[i + 4];
        file[i].f_host = argv[2];
        file[i].f_flags = 0;
    }
    printf("nfiles = %d\n", nfiles);

    home_page(argv[2], argv[3]);
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    maxfd = -1;
    nlefttoread = nlefttoconn = nfiles;
    nconn = 0;

    while (nlefttoread > 0) {
        while (nconn < maxnconn && nlefttoconn > 0) {
            // find a file to read
            for (i = 0; i < nfiles; i++) {
                if (file[i].f_flags == 0) {
                    break;
                }
            }
            if (i == nfiles) {
                err_quit("nlefttoconn = %d but nothing found", nlefttoconn);
            }
            start_connect(&file[i]);
            nconn++;
            nlefttoconn++;
        }
        rs = rset;
        ws = wset;
        if ((n = select(maxfd + 1, &rs, &ws, NULL, NULL)) < 0) {
            err_sys("select error");
        }

        for (i = 0; i < nfiles; i++) {
            flags = file[i].f_flags;
            if (flags == 0 || flags & F_DONE) {
                continue;
            }
            fd = file[i].f_fd;
            if (flags & F_CONNECTING && (FD_ISSET(fd, &rs) || FD_ISSET(fd, &ws))) {
                n = sizeof(error);
                if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &n) < 0 || error != 0) {
                    printf("error: %d(%s)\n", error, strerror(error));
                    err_ret("nonblocking connect failed for %s", file[i].f_name);
                }
                // connection established
                printf("connection established for %s\n", file[i].f_name);
                FD_CLR(fd, &wset); // no more writeablility test
                write_get_cmd(&file[i]); // write the GET command
            } else if (flags & F_READING && FD_ISSET(fd, &rs)) {
                if ((n = read(fd, buf, sizeof(buf))) == 0) {
                    printf("end-of-file on %s\n", file[i].f_name);
                    close(fd);
                    file[i].f_flags = F_DONE; // clears F_READING
                    nconn--;
                    nlefttoread--;
                } else {
                    printf("read %d bytes from %s\n", n, file[i].f_name);
                }
            }
        }
    }
    exit(0);
}

// $ sudo systemctl start nginx
// $ sudo systemctl start php-fpm

// $ gcc web.c home_page.c ../Chapter11/tcp_connect.c  ../Chapter03/writen.c start_connect.c ../Chapter11/host_serv.c write_get_cmd.c ../lib/error.c -o web
// $ ls -alh /data/www/
// -rw-r--r-- 1 fanbin fanbin  134 Jul 11 17:16 index.php
// $ ./web 3 127.0.0.1 / index.php index.php index.php
// nfiles = 3
// read 325 bytes of home page
// end-of-file on home page
// start_connect for index.php, fd 3
// start_connect for index.php, fd 4
// start_connect for index.php, fd 5
// connection established for index.php
// wrote 27 bytes for index.php
// connection established for index.php
// wrote 27 bytes for index.php
// connection established for index.php
// wrote 27 bytes for index.php
// read 325 bytes from index.php
// read 325 bytes from index.php
// read 325 bytes from index.php
// end-of-file on index.php
// end-of-file on index.php
// end-of-file on index.php
