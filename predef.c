#include "predef.h"

Function* new_internal_func(const char* name, Function* context)
{
    Function* f = malloc(sizeof(Function));
    f->name = name;
    f->funcs = llist_empty;
    f->labels = new_strmap(Label*, LabelMap);
    f->statms = llist_empty;
    f->vars = llist_empty;
    f->params = llist_empty;
    f->internal = true;
    f->context = context;
    
    context->funcs = llist_prepend(f, context->funcs);
    
    return f;
}

void add_param(Function* fun, const char* name, VarType type)
{
    Variable* var = malloc(sizeof(*var));
    var->name = name;
    var->type = type;
    
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