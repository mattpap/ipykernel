#ifndef __IALDOR_ZEROMQ_H__
#define __IALDOR_ZEROMQ_H__

#include <stdbool.h>

char* srecv(void* socket);
int ssend(void* socket, const char* string, bool more);

#endif // __IALDOR_ZEROMQ_H__
