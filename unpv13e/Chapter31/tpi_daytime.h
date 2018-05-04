#ifndef __tpi_daytime_h
#define __tpi_daytime_h

#include "unpxti.h"
#include <sys/stream.h>
#include <sys/tihdr.h>

void tpi_bind(int, const void *, size_t);
void tpi_connect(int, const void *, size_t);
ssize_t tip_read(int, void *, size_t);
void tpi_close(int);

#endif // __tpi_daytime_h
