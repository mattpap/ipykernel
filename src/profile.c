#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "profile.h"
#include "json.h"
#include "util.h"

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
    profile->signature_scheme = strdup("hmac-sha256");
}

void init_profile(Profile* profile, const char* existing) {
    FILE* file;
    char name[100];
    json_t* json;

    if (existing == NULL) {
        sprintf(name, "profile-%d.json", getpid());
        printf("connect ipython with --existing %s\n", name);

        default_profile(profile);

        json = json_object();
        json_object_set(json, "ip", json_string(profile->ip));
        json_object_set(json, "transport", json_string(profile->transport));
        json_object_set(json, "iopub_port", json_integer(profile->iopub_port));
        json_object_set(json, "shell_port", json_integer(profile->shell_port));
        json_object_set(json, "control_port", json_integer(profile->control_port));
        json_object_set(json, "stdin_port", json_integer(profile->stdin_port));
        json_object_set(json, "hb_port", json_integer(profile->hb_port));
        json_object_set(json, "key", json_string(profile->key));
        json_object_set(json, "signature_scheme", json_string(profile->signature_scheme));

        file = fopen(name, "w");
        json_dumpf(json, file, 0);
        json_decref(json);
        fclose(file);
    } else {
        file = fopen(existing, "r");

        if (!file) {
            fprintf(stderr, "error: can't open %s\n", existing);
            exit(1);
        }

        json = json_loadf(file, 0, NULL);

        if (!json) {
            fprintf(stderr, "error: can't parse JSON from %s\n", existing);
            exit(1);
        }

        fclose(file);

        if (!json_is_object(json)) {
            fprintf(stderr, "error: expected a JSON object, got %s\n", json_strof(json));
            exit(1);
        }

        profile->ip = json_get_string_key(json, "ip");
        profile->transport = json_get_string_key(json, "transport");
        profile->iopub_port = json_get_integer_key(json, "iopub_port");
        profile->shell_port = json_get_integer_key(json, "shell_port");
        profile->control_port = json_get_integer_key(json, "control_port");
        profile->stdin_port = json_get_integer_key(json, "stdin_port");
        profile->hb_port = json_get_integer_key(json, "hb_port");
        profile->key = json_get_string_key(json, "key");
        profile->signature_scheme = json_get_string_key(json, "signature_scheme");

        json_decref(json);
    }
}

void free_profile(Profile* profile) {
    free(profile->ip);
    free(profile->transport);
    free(profile->key);
    free(profile->signature_scheme);
}
