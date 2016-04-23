#include <string.h>
#include "strmap.h"


int _strmap_item_cmp(void const* old, void const* newe, size_t size)
{
    typedef _strmap_item(long) dummy;
    dummy const* o = old;
    dummy const* n = newe;
    
    return strcmp(o->name, n->name);
}
