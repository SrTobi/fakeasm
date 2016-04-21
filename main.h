#ifndef _FAKEASM_MAIN_H
#define _FAKEASM_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "llist.h"

int yylex(void);
int yyparse(void);
FILE *yyin;

void yyerrerf(const char*s, ...);
void yyerror(const char *s);

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
llist_decl(Variable*, VarList);
llist_decl(struct Function*, FunList);
llist_decl(Argument, ArgList);

typedef struct Statement{
    const char *funname;
    struct Function* fun;
    ArgList args;
    bool wasexpr;
} Statement;

llist_decl(Statement*, StatementList);
llist_decl(Label*, LabelList);

typedef struct Function {
    const char *name;
    struct Function* context;
    VarList params;
    VarList vars;
    FunList funcs;
    StatementList statms;
    LabelList labels;
    bool internal;
} Function;



#endif