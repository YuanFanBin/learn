#include "apue.h"
#include "apue_db.h"
#include <fcntl.h>

/* gcc apue.h apue_err.c apue_db.h db.c figure-14.5.c figure-20.3.c */
int
main(void)
{
    DBHANDLE    db;

    if ((db = db_open("db4", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) == NULL)
        err_sys("db_open error");

    if (db_store(db, "Alpha", "data1", DB_INSERT) != 0)
        err_quit("db_store error for alpha");
    if (db_store(db, "beta", "Data for beta", DB_INSERT) != 0)
        err_quit("db_store error for beta");
    if (db_store(db, "gamma", "record3", DB_INSERT) != 0)
        err_quit("db_store error for gamma");

    db_close(db);
    exit(0);
}

/*
 * 只是抄完书上代码，具体测试demo，优化建议的实现均未编写code。
 */
