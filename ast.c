#include <stdlib.h>
#include "ast.h"
#include "main.h"



Function* new_function(const char* name, Function* context)
{
    Function* f = malloc(sizeof(*f));
    f->name = name;
    f->funcs = new_strmap(Function*, FunMap);
    f->labels = new_strmap(Label*, LabelMap);
    f->statms = array_new(Statement*, 0);
    f->vars = new_strmap(Variable*, VarMap);
    f->params = array_new(Variable*, 0);
    f->internal = false;
    f->context = context;
    
    if(context != NULL)
    {
        if(strmap_find(context->funcs, name) != NULL)
            yyerrerf("A function with the name '%s' has already been defined in function '%s'.", name, context->name);
        
        strmap_insert(context->funcs, name, f);
    }
    
    return f; 
}

void function_add_parameter(Function* context, Variable* var)
{
    assert(array_len(context->params) == strmap_len(context->vars));
    
    if(strmap_find(context->vars, var->name))
    {
        yyerrerf("A variable with the name '%s' has already been defined in function '%s'.", var->name, context->name);
    }
    
    strmap_insert(context->vars, var->name, var);
    array_push(context->params, Variable*, var);
}

Statement* new_statement(const char* funname, ArgArray args)
{
    Statement* statm = malloc(sizeof(*statm));
    statm->args = args;
    statm->fun = NULL;
    statm->funname = funname;
    statm->wasexpr = false;
    
    return statm;
}

Variable* new_variable(const char* name, VarType type)
{
    Variable* v = malloc(sizeof(*v));
    v->name = name;
    v->type = type;
    return v;
}

Label* new_label(const char* name)
{
    Label* l = malloc(sizeof(*l));
    l->name = name;
    l->statm = NULL;
    
    return l;
}