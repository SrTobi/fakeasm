#include "parse.h"

FunList funstack = llist_empty;
LabelList curlabels = llist_empty;

void push_function(const char* name, VarList params)
{
    Function* f = malloc(sizeof(Function));
    f->name = name;
    f->funcs = llist_empty;
    f->labels = new_strmap(Label*, LabelMap);
    f->statms = llist_empty;
    f->vars = params;
    f->params = params;
    f->internal = false;


    Function* context = NULL;
    if(funstack != llist_empty)
    {
        context = llist_head(funstack);
        
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
    f->context = context;
    
    funstack = llist_prepend(f, funstack);
}

void pop_function()
{
    funstack = llist_tail(funstack);
    curlabels = llist_empty;
}

Function* cur_func()
{
    return llist_head(funstack);
}

void push_label(const char* name)
{
    Function* curf = cur_func();
    
    // check if label already exists
    Label** foundl = strmap_find(curf->labels, name);
    if(foundl != NULL)
        yyerrerf("A label with the name '%s' has already been defined in function '%s'.", name, curf->name);
    
    /*llist_foreach(LabelList, curf->labels, l)
    {
        if(strcmp(l->name, name) == 0)
        {
            
        }
    }*/    
    
    Label* l = malloc(sizeof(*l));
    l->name = name;
    l->statm = NULL;
    curlabels = llist_prepend(l, curlabels);
    
    strmap_insert(curf->labels, name, l);
    //curf->labels = llist_prepend(l, curf->labels);
}

Variable* create_variable(const char* name)
{
    Variable* v = malloc(sizeof(*v));
    v->name = name;
    v->type = vt_int;
    return v;
}

Variable* create_parameter(const char* name, VarType vartype)
{
    Variable* v = create_variable(name);
    v->type = vartype;
    return v;
}

Statement* push_funcall(const char* funname, ArgList args)
{
    Function* curf = cur_func();
    Statement* statm = malloc(sizeof(*statm));
    statm->args = args;
    statm->fun = NULL;
    statm->funname = funname;
    statm->wasexpr = false;
    
    // set all labels and pop
    while(curlabels != llist_empty)
    {
        llist_head(curlabels)->statm = statm;
        curlabels = llist_tail(curlabels);
    }
    
    // register
    curf->statms = llist_prepend(statm, curf->statms);
    return statm;
}

Argument create_iconst_arg(int val)
{
    Argument arg;
    arg.type = at_iconst;
    arg.arg.iconst = val;
    arg.wasexpr = false;
    return arg;
}

Argument create_label_or_var_arg(const char* name)
{
    Argument arg;
    arg.type = at_var_or_label;
    arg.arg.refname = name;
    arg.wasexpr = false;
    return arg;
}

int tmpvarcounter = 0;
Argument tmpargument()
{
    char* buffer = malloc(16);
    sprintf(buffer, "tmpv#%03i", tmpvarcounter++);
    Argument ta = create_label_or_var_arg(buffer);
    ta.wasexpr = true;
    return ta;
}

Argument create_unary_arg(const char* funname, Argument input)
{
    Argument output = tmpargument();
    ArgList arguments = llist_new(ArgList, input);
    arguments = llist_prepend(output, arguments);
    Statement* s = push_funcall(funname, arguments);
    s->wasexpr = true;
    
    output.wasexpr = false;
    return output;
}

Argument create_binary_arg(const char* funname, Argument left, Argument right)
{
    Argument output = tmpargument();
    ArgList arguments = llist_new(ArgList, left);
    arguments = llist_prepend(right, arguments);
    arguments = llist_prepend(output, arguments);
    Statement* s = push_funcall(funname, arguments);
    s->wasexpr = true;
    
    output.wasexpr = false;
    return output;
}