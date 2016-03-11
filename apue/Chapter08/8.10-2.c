#include "apue.h"
#include <unistd.h> /* environ */

/* gcc 8.16-2.c apue.h apue_err.c */
int
main(int argc, char *argv[])
{
    int i;
    char **ptr;
    extern char **environ;

    for (i = 0; i < argc; i++) { /* echo all command-line args */
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    printf("PATH = %s\n", getenv("PATH"));
    for (i = 0; environ[i] != NULL; i++) { /* echo all env strings */
        //printf("environ[%d]: %s\n", i, environ[i]);
    }

    exit(0);
}
