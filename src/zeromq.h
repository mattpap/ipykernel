#ifndef __IPYKERNEL_ZEROMQ_H__
#define __IPYKERNEL_ZEROMQ_H__

#include <stdbool.h>

char* srecv(void* socket);
int ssend(void* socket, const char* string, bool more);

#endif // __IPYKERNEL_ZEROMQ_H__
