#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <jansson.h>

#include "profile.h"
#include "json.h"
#include "uuid.h"

void default_profile(Profile* profile) {
    int port0 = 5678;
    profile->ip = strdup("127.0.0.1");
    profile->transport = strdup("tcp");
    profile->stdin_port = port0;
    profile->control_port = port0+1;
    profile->hb_port = port0+2;
    profile->shell_port = port0+3;
    profile->iopub_port = port0+4;
    profile->key = uuid4();
    profile->signature_scheme = strdup("sha256");
}

void init_profile(Profile* profile, const char* existing) {
    FILE* file;
    char name[100];
    json_t* json;
    json_t* value;

    if (existing == NULL) {
        sprintf(name, "profile-%d.json", getpid());
        printf("connect ipython with --existing %s\n", name);

        default_profile(profile);

        json = json_pack(
            "{s:s,s:s,s:i,s:i,s:i,s:i,s:i,s:s,s:s}",
            "ip", profile->ip,
            "transport", profile->transport,
            "iopub_port", profile->iopub_port,
            "shell_port", profile->shell_port,
            "control_port", profile->control_port,
            "stdin_port", profile->stdin_port,
            "hb_port", profile->hb_port,
            "key", profile->key,
            "signature_scheme", profile->signature_scheme);

        file = fopen(name, "w");
        json_dumpf(json, file, 0);
        json_decref(json);
        fclose(file);
    } else {
        file = fopen(existing, "r");
        json = json_loadf(file, 0, NULL);
        fclose(file);

        if (!json_is_object(json)) {
            fprintf(stderr, "error: expected a JSON object, got %s\n", json_strof(json));
            json_decref(json);
            exit(1);
        }

#define GET_STRING_KEY(json, value, key, dest)                                                       \
        value = json_object_get(json, key);                                                          \
        if (!json_is_string(value)) {                                                                \
            fprintf(stderr, "error: \"%s\" key must be a string, got %s\n", key, json_strof(value)); \
            json_decref(json);                                                                       \
            exit(1);                                                                                 \
        }                                                                                            \
        dest = strdup(json_string_value(value));

#define GET_INT_KEY(json, value, key, dest)                                                            \
        value = json_object_get(json, key);                                                            \
        if (!json_is_integer(value)) {                                                                 \
            fprintf(stderr, "error: \"%s\" key must be an integer, got %s\n", key, json_strof(value)); \
            json_decref(json);                                                                         \
            exit(1);                                                                                   \
        }                                                                                              \
        dest = json_integer_value(value);

        GET_STRING_KEY(json, value, "ip", profile->ip);
        GET_STRING_KEY(json, value, "transport", profile->transport);
        GET_INT_KEY(json, value, "iopub_port", profile->iopub_port);
        GET_INT_KEY(json, value, "shell_port", profile->shell_port);
        GET_INT_KEY(json, value, "control_port", profile->control_port);
        GET_INT_KEY(json, value, "stdin_port", profile->stdin_port);
        GET_INT_KEY(json, value, "hb_port", profile->hb_port);
        GET_STRING_KEY(json, value, "key", profile->key);
        GET_STRING_KEY(json, value, "signature_scheme", profile->signature_scheme);

#undef GET_INT_KEY
#undef GET_STRING_KEY

        json_decref(json);
    }
}

void free_profile(Profile* profile) {
    free(profile->ip);
    free(profile->transport);
    free(profile->key);
    free(profile->signature_scheme);
}
