#ifndef __IALDOR_INTERPRETER_H__
#define __IALDOR_INTERPRETER_H__

void init_interpreter(int argc, char** argv);
void free_interpreter();

void eval(char* code, char** out, char** err);

#endif // __IALDOR_INTERPRETER_H__
