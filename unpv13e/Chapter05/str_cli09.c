#include "../lib/error.h"
#include "sum.h"
#include <stdio.h>  /* FILE */
#include <string.h> /* strlen */
#include <unistd.h> /* write */

#define MAXLINE 4096

// ../Chapter03/readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);

void str_cli(FILE *fp, int sockfd)
{
    char            sendline[MAXLINE];
    struct args     args;
    struct result   result;

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (sscanf(sendline, "%ld%ld", &args.arg1, &args.arg2) != 2) {
            printf("invalid input: %s", sendline);
            continue;
        }
        if (write(sockfd, &args, sizeof(args)) != sizeof(args)) {
            err_sys("write error");
        }
        if (read(sockfd, &result, sizeof(result)) == 0) {
            err_quit("str_cli: server terminated prematurely");
        }
        printf("%ld\n", result.sum);
    }
}
