#ifndef _FAKEASM_LLIST_H
#define _FAKEASM_LLIST_H

#include <stddef.h>

#define _llist_offset(list)         (int)(((void*)&(list)->value)-(void*)list)
#define llist_decl(type, name)      typedef struct name##decl { struct name##decl *tail; type value; } *name;
#define llist_new(listtype, elem)   llist_prepend(elem, (listtype)llist_empty)
#define llist_empty                 (NULL)
#define llist_head(list)            (list->value)
#define llist_tail(list)            (list->tail)
#define llist_prepend(elem, list)   _llist_impl_prepend(list, &elem, sizeof(*list), sizeof(elem), _llist_offset(list))
#define llist_reverse(list)         _llist_impl_reverse(list, sizeof(*list))
#define llist_foreach(listtype, list, target)  for(listtype _idx = list; _idx != llist_empty? (target = llist_head(_idx), true) : false; _idx = llist_tail(_idx))

void *_llist_impl_prepend(void* list, void* elemptr, int listsize, int elemsize, int offset);
void *_llist_impl_reverse(void* list, int listsize);

int llist_size(void* list);
//void* llist_toarray(void*list);

//typedef struct name { int value; struct name *next; } name;
#endif