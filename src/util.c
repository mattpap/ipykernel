#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <uuid/uuid.h>
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

char* srecv(void* socket) {
    zmq_msg_t message;

    zmq_msg_init(&message);
    int rc = zmq_recv(socket, &message, 0);

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

    int rc = zmq_send(socket, &message, more ? ZMQ_SNDMORE : 0);

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
