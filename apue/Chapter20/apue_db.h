#ifndef _APUE_DB_H
#define _APUE_DB_H

typedef     void * DBHANDLE;

DBHANDLE    db_open(const char *pathname, int oflag, ... /* int mode */);
void        db_close(DBHANDLE);
char       *db_fetch(DBHANDLE db, const char *key);
int         db_store(DBHANDLE db, const char *key, const char *data, int flag);
int         db_delete(DBHANDLE db, const char *key);
void        db_rewind(DBHANDLE db);
char       *db_nextrec(DBHANDLE db, char *key);

/*
 * Flags for db_store().
 */
#define DB_INSERT   1   /* insert new record only */
#define DB_REPLACE  2   /* replace existing record */
#define DB_STORE    3   /* replace or insert */

/*
 * Implementation limits.
 */
#define IDXLEN_MIN  6       /* key, sep, start, sep, length, \n */
#define IDXLEN_MAX  1024    /* arbitrary */
#define DATLEN_MIN  2       /* data byte, newline */
#define DATLEN_MAX  10124   /* arbitrary. */

#endif /* _APUE_DB_H */
