#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "options.h"

void init_options(int* argc, char*** argv, Options* options) {
    int c;

    options->verbose = false;
    options->parent = false;
    options->profile = NULL;

    while (1) {
        static struct option long_options[] = {
            {"verbose", no_argument,       NULL, 'v'},
            {"parent",  no_argument,       NULL, 'p'},
            {"profile", required_argument, NULL, 'P'},
            {0, 0, 0, 0},
        };

        int option_index = 0;
        c = getopt_long_only(*argc, *argv, "vpP:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
        case 'v':
            options->verbose = true;
            break;
        case 'p':
            options->parent = true;
            break;
        case 'P':
            options->profile = strdup(optarg);
            break;
        case '?':
            break;
        default:
            abort();
        }
    }

    *argc -= optind;
    *argv += optind;
}
