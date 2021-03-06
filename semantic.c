#include <assert.h>
#include "main.h"

Function* resolve_funcall(Function* fun, const char* name)
{
    if(fun == NULL)
        return NULL;
    
    Function** inner;
    if((inner = strmap_find(fun->funcs, name)) != NULL)
    {
        return *inner;
    }
    
    return resolve_funcall(fun->context, name);
}

Variable* make_variable(Function*fun, const char* name)
{   
    // make var
    Variable *var = new_variable(name, vt_int);
    strmap_insert(fun->vars, name, var);
    return var;
}

Variable* resolve_variable(Function* fun, const char* name)
{
    Variable** var;
    if((var = strmap_find(fun->vars, name)) != NULL)
        return *var;
    
    return NULL;
}

Label* resolve_label(Function* fun, const char* name)
{
    Label** optL = strmap_find(fun->labels, name);
    return optL == NULL? NULL : *optL;
}

bool ast_resolve_funcalls(Function* fun)
{
    bool ok = true;
    Statement* s;
    array_foreach(fun->statms, Statement*, s)
    {
        // resolve funcalls
        s->fun = resolve_funcall(fun, s->funname);
        if(s->fun == NULL)
        {
            ok = false;
            printf("Error: Could not resolve function call to '%s' un function '%s'\n", s->funname, fun->name);
        } else {
            
            // resolve arguments
            int args_count = array_len(s->args);
            int param_count = array_len(s->fun->params);
            if(args_count != param_count)
            {
                ok = false;
                printf("Error: Function call to '%s' in function '%s'  has not the correct number of arguments: %i provided, %i expected\n", s->funname, fun->name, args_count, param_count);
            }
            
            Argument* arg = array_begin(s->args);
            Variable** pp = array_begin(s->fun->params);
            int i = 1;
            while(arg != array_end(s->args))
            {
                Variable* param = *pp;
                
                if(arg->wasexpr && param->type != vt_outint)
                {
                    ok = false;
                    printf("Error: Expected parameter %i of function '%s' to output something, in order to be used as expression in function '%s'\n", i, s->funname, fun->name);
                }
                
                switch(arg->type)
                {
                    case at_iconst:
                        switch(param->type)
                        {
                        case vt_label:
                            ok = false;
                            printf("Error: Expected label as argument %i in call to '%s' in function '%s'\n", i, s->funname, fun->name);
                            break;
                        case vt_outint:
                            ok = false;
                            printf("Error: Can not reference const value as argument %i in call to '%s' in function '%s'\n", i, s->funname, fun->name);
                            break;
                        case vt_int:
                            break;
                        }
                        break;
                    case at_var_or_label:
                        /*{
                            const char* argname = arg->arg.refname;
                            Label* label = resolve_label(fun, argname);
                            Variable* var = resolve_variable(fun, argname);
                            
                            if(param->type == vt_label)
                            {
                                if(label != NULL)
                                {
                                    arg->type = at_label;
                                    arg->arg.label = label;
                                } else if({
                                    
                                }
                            }
                        }*/
                        
                        switch(param->type)
                        {
                            case vt_int:
                            case vt_outint:
                                {
                                    const char* vname = arg->arg.refname;
                                    if(resolve_label(fun, vname))
                                    {
                                       goto found_label_error;
                                    }
                                    
                                    Variable* var = resolve_variable(fun, vname);
                                    if(var == NULL)
                                        var = make_variable(fun, vname);
                                        
                                    if(var->type == vt_label)
                                    {
                                    found_label_error:
                                        ok = false;
                                        printf("Error: Expected variable but found label '%s' as argument %i in call to '%s' in function '%s'\n", vname, i, s->funname, fun->name);
                                    }else{
                                        arg->type = at_var;
                                        arg->arg.var = var;
                                    }
                                }
                                break;
                            case vt_label:
                                {
                                    const char* lname = arg->arg.refname;
                                    Label* label = resolve_label(fun, lname);
                                    if(label != NULL)
                                    {
                                        arg->type = at_label;
                                        arg->arg.label = label;
                                    }else{
                                        Variable* var = resolve_variable(fun, lname);
                                        if(var == NULL)
                                        {
                                            ok = false;
                                            printf("Error: Could not find label '%s' in function '%s' to provide as argument %i in call to '%s'\n", lname, fun->name, i, s->funname);
                                            break;
                                        } else {
                                            if(var->type == vt_label)
                                            {
                                                arg->type = at_labelref;
                                                arg->arg.var = var;
                                            } else {
                                                printf("Error: Expected label but found variable '%s' as argument %i in call to '%s' in function '%s'\n", lname, i, s->funname, fun->name);
                                            }
                                        }
                                    }
                                } break;
                        }
                        break;
                    default:
                        assert(!"Argument already resolved?");
                }
                
                
                
                ++pp;
                ++arg;
                ++i;
            }
        }
    }
    
    Function* inner;
    strmap_foreach(fun->funcs, Function*, inner)
    {
        bool success = ast_resolve_funcalls(inner);
        if(!success)
        {
            ok = false;
        }
    }
    
    return ok;
}

void ast_analyze(Function* fun)
{
    if(!ast_resolve_funcalls(fun))
        yyerror("Semantic errors detected");
}