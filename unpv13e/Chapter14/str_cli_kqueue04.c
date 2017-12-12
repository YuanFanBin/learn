#include <fcntl.h>
#include <stdio.h>
#include <sys/event.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "../lib/error.h"

#define MAXLINE 4096 /* max text line length */

void str_cli(FILE *fp, int sockfd)
{
    int             kq, i, n, nev, stdineof = 0, isfile;
    char            buf[MAXLINE];
    struct kevent   kev[2];
    struct timtspec ts;
    struct stat     st;

    isfile = ((fstat(fileno(fp), &st) == 0) && (st.st_mode & S_IFMT) == S_IFREG);

    EV_SET(&kev[0], fileno(fp), EVFILT_READ, EV_ADD, 0, 0, NULL);
    EV_SET(&kev[1], sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

    if ((kq = kqueue()) == -1) {
        err_sys("kqueue error");
    }
    ts.tv_sec = tv.tv_nsec = 0;
    if (kevent(kq, kev, 2, NULL, 0, &ts) == -1) {
        err_sys("kevent error");
    }

    for (;;) {
        if ((nev = kevent(kq, NULL, 0, kev, 2, NULL)) == -1) {
            err_sys("kevent error");
        }
        for (i = 0; i < nev; i++) {
            if (kev[i].ident == sockfd) { // socket is readable
                if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                    if (stdineof == 1) {
                        return; // normal termination
                    } else {
                        err_quit("str_cli: server terminated prematurely");
                    }
                }
                write(fileno(stdout), buf, n);
            }
            if (kev[i].ident == fileno(fp)) { // input is raedable
                if ((n = read(fileno(fp), buf, MAXLINE)) == -1) {
                    err_sys("read error");
                }
                if (n > 0) {
                    write(sockfd, buf, n);
                }
                if (n == 0 || (isfile && n == kev[i].data)) {
                    stdineof = 1;
                    shutdown(sockfd, SHUT_WR); // send FIN
                    kev[i].flags = EV_DELETE;
                    kevent(kq, &kev[i], 1, NULL, 0, &ts); // remove kevent
                    continue;
                }
            }
        }
    }
}
