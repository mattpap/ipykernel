#ifndef __IPYKERNEL_INTERPRETER_H__
#define __IPYKERNEL_INTERPRETER_H__

typedef void (*EvaluateFunction)(const char* code, char** out, char** err);
typedef void (*CompleteFunction)(const char* line, int pos, StringList* matches, char** matched_text);

void set_evaluate_func(EvaluateFunction func);
void set_complete_func(CompleteFunction func);

void evaluate(const char* code, char** out, char** err);
void complete(const char* line, int pos, StringList* matches, char** matched_text);

#endif // __IPYKERNEL_INTERPRETER_H__
