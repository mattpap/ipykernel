#include <stdio.h>
#include <string.h>

#include "types.h"
#include "globals.h"
#include "options.h"
#include "profile.h"
#include "hmac.h"
#include "sockets.h"
#include "interpreter.h"
#include "communication.h"
#include "heartbeat.h"
#include "eventloop.h"

char* kernel_lang = "echo";

void evaluate_func(const char* input_code, FILE* out, FILE* err) {
    char* code = strdup(input_code);
    size_t len = strlen(code);

    if (code[len-1] == '\n')
        code[len-1] = '\0';

    fprintf(out, "echo: %s", code);
}

void complete_func(const char* line, int pos, StringList* matches, char** matched_text) {
    matches->list = NULL;
    matches->size = 0;
    *matched_text = NULL;
}

int main(int argc, char** argv) {
    init_options(&argc, &argv, &options);
    init_profile(&profile, options.profile);
    init_sockets(&sockets);
    init_hmac();
    init_heartbeat();

    set_evaluate_func(evaluate_func);
    set_complete_func(complete_func);

    send_status(state_starting);
    eventloop(sockets.requests);

    return 0;
}
