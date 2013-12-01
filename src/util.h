#ifndef __IALDOR_UTIL_H__
#define __IALDOR_UTIL_H__

#include <stdbool.h>

char* uuid4();
const char* json_strof(const json_t* json);
char* srecv(void* socket);
int ssend(void* socket, const char* string, bool more);
char* hmac(Profile* profile, const char* s1, ...);

#endif // __IALDOR_UTIL_H__