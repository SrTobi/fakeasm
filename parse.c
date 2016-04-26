#include "parse.h"

FunList funstack = llist_empty;
LabelList curlabels = llist_empty;

static void add_all_params(VarList params, Function* fun)
{
    if(params == llist_empty)
        return;

    add_all_params(llist_tail(params), fun);
    function_add_parameter(fun, llist_head(params));
}

void push_function(const char* name, VarList params)
{
    Function* context = NULL;
    if(funstack != llist_empty)
    {
        context = llist_head(funstack);
    }
    
    Function* f = new_function(name, context);
    
    add_all_params(params, f);
    
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
    
    Label* l = new_label(name);
    curlabels = llist_prepend(l, curlabels);
    
    strmap_insert(curf->labels, name, l);
    //curf->labels = llist_prepend(l, curf->labels);
}

Variable* create_variable(const char* name)
{
    return new_variable(name, vt_int);
}

Variable* create_parameter(const char* name, VarType vartype)
{
    return new_variable(name, vartype);
}

Statement* push_funcall(const char* funname, ArgArray args)
{
    Function* curf = cur_func();
    Statement* statm = new_statement(funname, args);
    
    // set all labels and pop
    while(curlabels != llist_empty)
    {
        llist_head(curlabels)->statm = statm;
        curlabels = llist_tail(curlabels);
    }
    
    // register
    array_push(curf->statms, Statement*, statm);
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
    ArgArray arguments = array_new(Argument, 2);
    arguments[0] = input;
    arguments[1] = output;
    Statement* s = push_funcall(funname, arguments);
    s->wasexpr = true;
    
    output.wasexpr = false;
    return output;
}

Argument create_binary_arg(const char* funname, Argument left, Argument right)
{
    Argument output = tmpargument();
    ArgArray arguments = array_new(Argument, 3);
    arguments[0] = left;
    arguments[1] = right;
    arguments[2] = output;
    Statement* s = push_funcall(funname, arguments);
    s->wasexpr = true;
    
    output.wasexpr = false;
    return output;
}