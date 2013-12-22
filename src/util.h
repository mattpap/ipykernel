#ifndef __IALDOR_UTIL_H__
#define __IALDOR_UTIL_H__

#include <stdbool.h>
#include <jansson.h>
#include "profile.h"

char* uuid4();
const char* json_strof(const json_t* json);
json_t* loads(const char* input);
char* dumps(const json_t* json);
char* json_get_string_key(const json_t* obj, const char* key);
int json_get_integer_key(const json_t* obj, const char* key);
bool json_get_bool_key(const json_t* obj, const char* key);
char* srecv(void* socket);
int ssend(void* socket, const char* string, bool more);
char* hmac(const char* s1, ...);

#endif // __IALDOR_UTIL_H__
