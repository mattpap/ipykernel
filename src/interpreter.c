#include <stdio.h>

#include "types.h"
#include "interpreter.h"

static EvaluateFunction evaluate_func = NULL;
static CompleteFunction complete_func = NULL;

EvaluateFunction set_evaluate_func(EvaluateFunction func) {
    EvaluateFunction old = evaluate_func;
    evaluate_func = func;
    return old;
}

CompleteFunction set_complete_func(CompleteFunction func) {
    CompleteFunction old = complete_func;
    complete_func = func;
    return old;
}

void evaluate(const char* code, char** out, char** err) {
    size_t out_size, err_size;

    FILE* fin = fmemopen((char*)code, strlen(code), "r");
    FILE* fout = open_memstream(out, &out_size);
    FILE* ferr = open_memstream(err, &err_size);

    (*evaluate_callback)(fin, fout, ferr);

    fclose(fin);
    fclose(fout);
    fclose(ferr);
}

void complete(const char* line, int pos, StringList* matches, char** matched_text) {
    (*complete_callback)(line, pos, matches, matched_text);
}
