#ifndef _FAKEASM_PRINT_H
#define _FAKEASM_PRINT_H

#include "main.h"

enum print_flags {
    pf_recfunc = 0x1,
    pf_labels = 0x2,
    pf_statments = 0x4,
    
    pf_all = pf_recfunc | pf_labels | pf_statments
};

void print_function(Function* fun, unsigned int flags);

#endif