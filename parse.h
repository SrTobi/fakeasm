#ifndef _FAKEASM_PARSE_H
#define _FAKEASM_PARSE_H

#include "main.h"

void push_function(const char* name, VarList params);
void pop_function();
Function* cur_func();
void push_label(const char* name);
Variable* create_variable(const char* name);
Variable* create_parameter(const char* name, VarType vartype);
Statement* push_funcall(const char* funname, ArgList args);
Argument create_iconst_arg(int val);
Argument create_label_or_var_arg(const char* name);
Argument create_unary_arg(const char* funname, Argument arg);
Argument create_binary_arg(const char* funname, Argument left, Argument right);


#endif