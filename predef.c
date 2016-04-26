#include "predef.h"

Function* new_internal_func(const char* name, Function* context)
{
    Function* f = new_function(name, context);
    f->internal = true;
    
    return f;
}

void add_param(Function* fun, const char* name, VarType type)
{
    Variable* var = new_variable(name, type);
    
    fun->vars = llist_prepend(var, fun->vars);
    fun->params = llist_prepend(var, fun->params);
}

void register_internals(Function* ctx)
{
    Function* mov = new_internal_func("mov", ctx);
    add_param(mov, "src", vt_int);
    add_param(mov, "dest", vt_outint);
    
    Function* gto = new_internal_func("goto", ctx);
    add_param(gto, "target", vt_label);
    
    Function* jz = new_internal_func("jz", ctx);
    add_param(jz, "tocmp", vt_int);
    add_param(jz, "target", vt_label);
}