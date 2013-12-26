#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

static const char* json_type_strings[] = {
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

json_t* loads(const char* input) {
    if (input == NULL || strlen(input) == 0)
        return NULL;

    json_error_t error;
    json_t* json = json_loads(input, 0, &error);

    if (json == NULL) {
        fprintf(stderr, "error: unable to decode JSON: %s\n", error.text);
        exit(1);
    }

    return json;
}

char* dumps(const json_t* json) {
    if (json != NULL)
        return json_dumps(json, JSON_COMPACT);
    else
        return strdup("");
}

char* json_get_string_key(const json_t* obj, const char* key) {
    json_t* value = json_object_get(obj, key);
    if (value == NULL) {
        fprintf(stderr, "error: missing required \"%s\" key\n", key);
        exit(1);
    }
    if (json_is_null(value)) {
        return NULL;
    }
    if (!json_is_string(value)) {
        fprintf(stderr, "error: \"%s\" key must be a string, got %s\n", key, json_strof(value));
        exit(1);
    }
    return strdup(json_string_value(value));
}

int json_get_integer_key(const json_t* obj, const char* key) {
    json_t* value = json_object_get(obj, key);
    if (value == NULL) {
        fprintf(stderr, "error: missing required \"%s\" key\n", key);
        exit(1);
    }
    if (!json_is_integer(value)) {
        fprintf(stderr, "error: \"%s\" key must be an integer, got %s\n", key, json_strof(value));
        exit(1);
    }
    return json_integer_value(value);
}

bool json_get_bool_key(const json_t* obj, const char* key) {
    json_t* value = json_object_get(obj, key);
    if (value == NULL) {
        fprintf(stderr, "error: missing required \"%s\" key\n", key);
        exit(1);
    }
    if (!json_is_boolean(value)) {
        fprintf(stderr, "error: \"%s\" key must be a boolean, got %s\n", key, json_strof(value));
        exit(1);
    }
    return json_is_true(value);
}
