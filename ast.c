#include <stdlib.h>
#include "ast.h"
#include "main.h"



Function* new_function(const char* name, Function* context)
{
    Function* f = malloc(sizeof(*f));
    f->name = name;
    f->funcs = llist_empty;
    f->labels = new_strmap(Label*, LabelMap);
    f->statms = llist_empty;
    f->vars = llist_empty;
    f->params = llist_empty;
    f->internal = false;
    f->context = context;
    
    if(context != NULL)
    {
        Function* otherf;
        llist_foreach(FunList, context->funcs, otherf)
        {
            if(strcmp(otherf->name, name) == 0)
            {
                yyerrerf("A function with the name '%s' has already been defined in function '%s'.", name, context->name);
            }
        }
        
        context->funcs = llist_prepend(f, context->funcs);
    }
    
    return f; 
}

Statement* new_statement(const char* funname, ArgList args)
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