#ifndef _FAKEASM_AST_H
#define _FAKEASM_AST_H


#include <string.h>
#include <stdbool.h>

#include "llist.h"
#include "strmap.h"
#include "array.h"

typedef enum {
    vt_int,
    vt_outint,
    vt_label
} VarType;

typedef struct {
    const char *name;
    VarType type;
} Variable;

struct Statement;

typedef struct {
    const char *name;
    struct Statement* statm;
} Label;

typedef enum {
    at_var_or_label,
    at_var,
    at_iconst,
    at_label,
    at_labelref
} ArgType;

typedef struct {
    ArgType type;
    union {
        Variable* var;
        int iconst;
        Label* label;
        const char *refname;
    } arg;
    bool wasexpr;
} Argument;

struct Function;
array_decl(Variable*, VarArray);
strmap_decl(Variable*, VarMap);
llist_decl(Variable*, VarList);
strmap_decl(struct Function*, FunMap);
array_decl(Argument, ArgArray);

typedef struct Statement{
    const char *funname;
    struct Function* fun;
    ArgArray args;
    bool wasexpr;
} Statement;

array_decl(Statement*, StatementArray);
strmap_decl(Label*, LabelMap);

typedef struct Function {
    const char *name;
    struct Function* context;
    VarArray params;
    VarMap vars;
    FunMap funcs;
    StatementArray statms;
    LabelMap labels;
    bool internal;
} Function;

Function* new_function(const char* name, Function* context);
void function_add_parameter(Function* context, Variable* var);
Statement* new_statement(const char* funname, ArgArray args);
Variable* new_variable(const char* name, VarType type);
Label* new_label(const char* name);

#endif