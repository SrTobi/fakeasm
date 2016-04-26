#ifndef _FAKEASM_ARRAY_HPP
#define _FAKEASM_ARRAY_HPP

#include <libfirm/adt/array.h>

#define array_decl(type, name)              typedef type *name;
#define array_new(type, size)               NEW_ARR_FZ(type, size)
#define array_empty(arr)                    (ARR_LEN(arr) == 0)
#define array_first(arr)                    (arr)[0]
#define array_begin(arr)                    (arr)
#define array_end(arr)                      ((arr) + array_len(arr))
#define array_last(arr)                     (arr)[ARR_LEN(arr) - 1]
#define array_push(arr, type, val)          ARR_APP1(type, arr, val)
#define array_pop(arr, type)                ARR_RESIZE(type, arr, array_len(arr) - 1)
#define array_len(arr)                      ARR_LEN(arr)
#define array_foreach(arr, type, target)    for(type* _idx = array_begin(arr); _idx == array_end(arr)? false : (target = *_idx, true); ++_idx)

#endif