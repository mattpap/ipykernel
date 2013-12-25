#ifndef __IALDOR_INTERPRETER_H__
#define __IALDOR_INTERPRETER_H__

void init_interpreter(int argc, char** argv);
void free_interpreter();

void evaluate(const char* code, char** out, char** err);
void complete(const char* line, int pos, char*** matches, int* count, char** text);

#endif // __IALDOR_INTERPRETER_H__
