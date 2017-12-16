#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../lib/error.h"

#define MAXLINE     4096        /* max text line length */
#define max(a, b)   ((a) > (b) ? (a) : (b))

char *gf_time(void);

void str_cli(FILE *fp, int sockfd)
{
    // 1
    // ----------------------------------------------
    int      maxfdp1, val, stdineof = 0;
    ssize_t  n, nwritten;
    fd_set   rset, wset;
    char     to[MAXLINE], fr[MAXLINE];
    char    *toiptr, *tooptr, *friptr, *froptr;

    // sockfd
    if ((val = fcntl(sockfd, F_GETFL, 0)) < 0) {
        err_sys("fcntl error");
    }
    if (fcntl(sockfd, F_SETFL, val | O_NONBLOCK) < 0) {
        err_sys("fcntl error");
    }

    // stdin
    if ((val = fcntl(STDIN_FILENO, F_GETFL, 0)) < 0) {
        err_sys("fcntl error");
    }
    if (fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK) < 0) {
        err_sys("fcntl error");
    }

    // stdout
    if ((val = fcntl(STDOUT_FILENO, F_GETFL, 0)) < 0) {
        err_sys("fcntl error");
    }
    if (fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK) < 0) {
        err_sys("fcntl error");
    }

    toiptr = tooptr = to;
    friptr= froptr = fr;

    maxfdp1 = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;
    for (;;) {
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        if (stdineof == 0 && toiptr < &to[MAXLINE]) {
            FD_SET(STDIN_FILENO, &rset); // read from stdin
        }
        if (friptr < &fr[MAXLINE]) {
            FD_SET(sockfd, &rset); // read from socket
        }
        if (tooptr != toiptr) {
            FD_SET(sockfd, &wset); // data to write to socket
        }
        if (froptr != friptr) {
            FD_SET(STDOUT_FILENO, &wset); // data to write to stdout
        }
        if (select(maxfdp1, &rset, &wset, NULL, NULL) < 0) {
            err_sys("select error");
        }
        // 2
        // ----------------------------------------------
        if (FD_ISSET(STDIN_FILENO, &rset)) {
            if ((n = read(STDIN_FILENO, toiptr, &to[MAXLINE] - toiptr)) < 0) {
                if (errno != EWOULDBLOCK) {
                    err_sys("read error on stdin");
                }
            } else if (n == 0) {
                fprintf(stderr, "%s: EOF on stdin\n", gf_time());
                stdineof = 1; // all done white stdin
                if (tooptr == toiptr) {
                    shutdown(sockfd, SHUT_WR); // send FIN
                }
            } else {
                fprintf(stderr, "%s: read %ld bytes from stdin\n", gf_time(), n);
                toiptr += n; // just read
                FD_SET(sockfd, &wset); // try and write to socket below
            }
        }
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0) {
                if (errno != EWOULDBLOCK) {
                    err_sys("read error on socket");
                }
            } else if (n == 0) {
                fprintf(stderr, "%s: EOF on socket\n", gf_time());
                if (stdineof) {
                    return ; // normal termination
                } else {
                    err_quit("str_cli: server terminated prematurely");
                }
            } else {
                fprintf(stderr, "%s: read %ld bytes from socket\n", gf_time(), n);
                friptr += n; // just read
                FD_SET(STDOUT_FILENO, &wset); // try and write below
            }
        }
        // 3
        // ----------------------------------------------
        if (FD_ISSET(STDOUT_FILENO, &wset) && ((n = friptr - froptr) > 0)) {
            if ((nwritten = write(STDOUT_FILENO, froptr, n)) < 0) {
                if (errno != EWOULDBLOCK) {
                    err_sys("write error to stdout");
                }
            } else {
                fprintf(stderr, "%s: wrote %ld bytes to stdout\n", gf_time(), nwritten);
                froptr += nwritten;
                if (froptr == friptr) {
                    froptr = friptr = fr; // back to begining of buffer
                }
            }
        }
        if (FD_ISSET(sockfd, &wset) && ((n = toiptr - tooptr) > 0)) {
            if ((nwritten = write(sockfd, tooptr, n)) < 0) {
                if (errno != EWOULDBLOCK) {
                    err_sys("write error to socket");
                }
            } else {
                fprintf(stderr, "%s: wrote %ld bytes to socket\n", gf_time(), nwritten);
                tooptr += nwritten;
                if (tooptr == toiptr) {
                    tooptr = toiptr = to; // back to begining of buffer
                    if (stdineof) {
                        shutdown(sockfd, SHUT_WR); // send FIN
                    }
                }
            }
        }
    }
}
