#ifndef __IALDOR_OPTIONS_H__
#define __IALDOR_OPTIONS_H__

#include <stdbool.h>

typedef struct Options {
    bool verbose;
    bool parent;
    char* profile;
} Options;

void init_options(int* argc, char*** argv, Options* options);

#endif // __IALDOR_OPTIONS_H__
