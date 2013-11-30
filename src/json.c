#include <jansson.h>

const char* json_type_strings[] = {
    "object",
    "array",
    "string",
    "integer",
    "real",
    "true",
    "false",
    "null",
};

const char* json_strof(const json_t* json) {
    switch json_typeof(json) {
        case JSON_OBJECT:  return json_type_strings[0];
        case JSON_ARRAY:   return json_type_strings[1];
        case JSON_STRING:  return json_type_strings[2];
        case JSON_INTEGER: return json_type_strings[3];
        case JSON_REAL:    return json_type_strings[4];
        case JSON_TRUE:    return json_type_strings[5];
        case JSON_FALSE:   return json_type_strings[6];
        case JSON_NULL:    return json_type_strings[7];
    }
}
