#ifndef _FAKEASM_PRINT_H
#define _FAKEASM_PRINT_H

#include "main.h"

enum print_flags {
    pf_recfunc = 0x1,
    pf_labels = 0x2,
    pf_statments = 0x4,
    pf_variables = 0x8,
    pf_fullfuncname = 0x10,
    pf_param_types = 0x20,
    pf_var_types = 0x40,
    
    pf_all = pf_recfunc | pf_labels | pf_statments | pf_variables | pf_fullfuncname | pf_param_types | pf_var_types
};

void print_function(Function* fun, unsigned int flags);

#endif