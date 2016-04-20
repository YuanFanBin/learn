#include "figure-17.20.h"

char    errmsg[MAXLINE];
int     oflag;
char   *pathname;

/* gcc apue.h apue_err.c figure-14.24.c figure-17.20.h figure-17.12.c figure-17.13.c figure-17.21.c figure-17.22.c figure-17.23.c figure-17.24.c -o opend */
int
main(void)
{
    int     nread;
    char    buf[MAXLINE];

    for ( ; ; ) {   /* read arg buffer from client, process request */
        if ((nread = read(STDIN_FILENO, buf, MAXLINE)) < 0)
            err_sys("read error on stream pipe");
        else if (nread == 0)
            break;  /* client has closed the stream pipe */
        handle_request(buf, nread, STDOUT_FILENO);  /* figure-17.22.c */
    }
    exit(0);
}
