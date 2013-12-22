#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <uuid/uuid.h>
#include <jansson.h>
#include <zmq.h>

#include "globals.h"

#define UUID_STRING_SIZE 36

char* uuid4() {
    uuid_t uuid;
    char* string = malloc(UUID_STRING_SIZE+1);
    uuid_generate_random(uuid);
    uuid_unparse(uuid, string);
    return string;
}

static const char* json_type_strings[] = {
    "object",
    "array",
    "string",
    "integer",
    "real",
    "true",
    "false",
    "null",
    "???",
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
        default:           return json_type_strings[8];
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
    if (!json_is_string(value)) {
        fprintf(stderr, "error: \"%s\" key must be a string, got %s\n", key, json_strof(value));
        exit(1);
    }
    return strdup(json_string_value(value));
}

int json_get_integer_key(const json_t* obj, const char* key) {
    json_t* value = json_object_get(obj, key);
    if (!json_is_integer(value)) {
        fprintf(stderr, "error: \"%s\" key must be an integer, got %s\n", key, json_strof(value));
        exit(1);
    }
    return json_integer_value(value);
}

bool json_get_bool_key(const json_t* obj, const char* key) {
    json_t* value = json_object_get(obj, key);
    if (!json_is_boolean(value)) {
        fprintf(stderr, "error: \"%s\" key must be a boolean, got %s\n", key, json_strof(value));
        exit(1);
    }
    return json_is_true(value);
}

char* srecv(void* socket) {
    zmq_msg_t message;

    zmq_msg_init(&message);
    int rc = zmq_msg_recv(&message, socket, 0);

    if (rc == -1) {
        fprintf(stderr, "error: failed to receive from ZMQ socket (errno=%d)\n", errno);
        exit(1);
    }

    int size = zmq_msg_size(&message);
    char* string = malloc(size+1);
    memcpy(string, zmq_msg_data(&message), size);
    string[size] = 0;

    zmq_msg_close(&message);
    return string;
}

int ssend(void* socket, const char* string, bool more) {
    zmq_msg_t message;
    int size = strlen(string);
    zmq_msg_init_size(&message, size);
    memcpy(zmq_msg_data(&message), string, size);

    int rc = zmq_msg_send(&message, socket, more ? ZMQ_SNDMORE : 0);

    if (rc == -1) {
        fprintf(stderr, "error: failed to send to ZMQ socket (errno=%d)\n", errno);
        exit(1);
    }

    zmq_msg_close(&message);
    return rc;
}

char* hmac(const char* s1, ...) {
    const EVP_MD* md;
    HMAC_CTX ctx;
    va_list args;
    char* s;
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int i, digest_size;
    char* hex_digest;

    OpenSSL_add_all_digests();

    const char* scheme = profile.signature_scheme;

    if (strncmp(scheme, "hmac-", 5) == 0)
        scheme += 5*sizeof(char);

    md = EVP_get_digestbyname(scheme);
    if (md == NULL) {
        fprintf(stderr, "error: \"%s\" digest algorithm not supported\n", scheme);
        exit(1);
    }

    HMAC_CTX_init(&ctx);
    HMAC_Init(&ctx, profile.key, strlen(profile.key), md);

    HMAC_Update(&ctx, (const unsigned char*)s1, strlen(s1));

    va_start(args, s1);
    while ((s = va_arg(args, char*))) {
        HMAC_Update(&ctx, (const unsigned char*)s, strlen(s));
    }
    va_end(args);

    HMAC_Final(&ctx, digest, &digest_size);
    HMAC_CTX_cleanup(&ctx);

    int len = 2*digest_size + 1;
    hex_digest = malloc(len);
    hex_digest[len-1] = 0;

    for (i = 0; i < digest_size; i++)
        sprintf(hex_digest + 2*i, "%02x", digest[i]);

    return hex_digest;
}
