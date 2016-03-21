#include "apue.h"

/* gcc apue.h figure-8.17.c -o echoall */
int
main(int argc, char *argv[])
{
    int             i;
    char          **ptr;
    extern char   **environ;

    for (i = 0; i < argc; ++i) {    /* echo all ecommand-line args */
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    for (ptr = environ; *ptr != 0; ptr++) { /* and all env strings */
        printf("%s\n", *ptr);
    }

    exit(0);
}
