#include <stdlib.h>
#include <string.h>

#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <uuid/uuid.h>

#include "globals.h"

#define UUID_STRING_SIZE 36

char* uuid4() {
    uuid_t uuid;
    char* string = malloc(UUID_STRING_SIZE+1);
    uuid_generate_random(uuid);
    uuid_unparse(uuid, string);
    return string;
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
