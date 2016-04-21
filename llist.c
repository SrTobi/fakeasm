#include <stdlib.h>
#include <string.h>

#include "llist.h"

llist_decl(long, dummyl);

void *_llist_impl_prepend(void* list, void* elemptr, int listsize, int elemsize, int offset)
{
    
    dummyl newelem = malloc(listsize);
    int* iptr = elemptr;
    
    newelem->tail = list;
    memcpy((void*)newelem + offset, elemptr, elemsize);
    
    return newelem;
}

void *_llist_impl_copy_with_tail(void* src, int listsize, void* tail)
{
    dummyl newlist = malloc(listsize);
    newlist->tail = tail;
    memcpy((void*)newlist + sizeof(void*), src + sizeof(void*), listsize - sizeof(void*));
    return newlist;
}

void *_llist_impl_reverse(void *list, int listsize)
{
    dummyl l = list;
    dummyl newl = llist_empty;
    
    while(l != llist_empty)
    {
        newl = _llist_impl_copy_with_tail(l, listsize, newl);
        l = llist_tail(l);
    }
    
    return newl;
}

int llist_size(void *list)
{
        dummyl l = list;
    int size = 0;
    while(l != llist_empty)
        ++size;
    
    return size;
}

/*void* llist_toarray(void *list, int elemsize)
{
    return NULL;
}*/