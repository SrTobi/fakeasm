#include "main.h"



Function* new_function(const char* name, Function* context)
{
    Function* f = malloc(sizeof(Function));
    f->name = name;
    f->context = context;
    f->funcs = llist_empty;
    f->labels = llist_empty;
    f->params = llist_empty;
    f->statms = llist_empty;
    f->vars = llist_empty;
    
    return f;
}