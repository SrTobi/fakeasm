#ifndef _FAKEASM_STRMAP_H
#define _FAKEASM_STRMAP_H

#include <libfirm/adt/set.h>
#include <libfirm/adt/hashptr.h>

#define _strmap_item(type)                  struct { const char* name; type value; }
#define _strmap_decltype(type)              struct { set* _innerset; _strmap_item(type)* _item_dummy; type* _typeptr_dummy; }
#define strmap_decl(type, name)             typedef _strmap_decltype(type) name;
#define new_strmap(type, maptype)           ((maptype){ new_set(&_strmap_item_cmp, 64), NULL, (type*)NULL})
#define strmap_len(map)                     set_count((map)._innerset)

#define strmap_insert(map, id, elem)        { \
                                                    char _item_buffer[sizeof(*(map)._item_dummy)]; \
                                                    (map)._item_dummy = (void*)_item_buffer; \
                                                    (map)._item_dummy->name = id; \
                                                    (map)._item_dummy->value = elem; \
                                                    _set_search((map)._innerset, (map)._item_dummy, sizeof(*(map)._item_dummy), hash_str(id), _set_insert); \
                                            }

#define strmap_find(map, id)                ( \
                                                ((map)._item_dummy = _set_search((map)._innerset, \
                                                                                ((map)._item_dummy = (void*)(&(id))), \
                                                                                sizeof(*((map)._item_dummy)), \
                                                                                hash_str(id), \
                                                                                _set_find)), \
                                                ((map)._item_dummy == NULL ? ((map)._typeptr_dummy = NULL) : &((map)._item_dummy->value))  \
                                            )

#define strmap_foreach(map, type, target)    for (void *_idx = set_first(void, map._innerset); _idx == NULL? false : (target = ((_strmap_item(type)*)_idx)->value, true); _idx = set_next(void, map._innerset))
                                      
int _strmap_item_cmp(void const* old, void const* newe, size_t size);


#endif