#ifndef __IPYKERNEL_JSON_H__
#define __IPYKERNEL_JSON_H__

#include <stdbool.h>
#include <jansson.h>

#ifndef json_boolean
#define json_boolean(val) ((val) ? json_true() : json_false())
#endif // json_boolean

#ifndef json_object_foreach
#define json_object_foreach(object, key, value) \
    for(void* iter = json_object_iter(object); \
        iter && (key = json_object_iter_key(iter)) && (value = json_object_iter_value(iter)); \
        iter = json_object_iter_next(object, iter))
#endif // json_object_foreach

#ifndef json_array_foreach
#define json_array_foreach(array, index, value) \
    for(index = 0; \
        index < json_array_size(array) && (value = json_array_get(array, index)); \
        index++)
#endif // json_array_foreach

const char* json_strof(const json_t* json);

json_t* loads(const char* input);
char* dumps(const json_t* json);

char* json_get_string_key(const json_t* obj, const char* key);
int json_get_integer_key(const json_t* obj, const char* key);
bool json_get_bool_key(const json_t* obj, const char* key);

#endif // __IPYKERNEL_JSON_H__
