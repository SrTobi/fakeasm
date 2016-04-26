#include "gen.h"
#include <libfirm/firm.h>

void build_firm_graph(Function* fun)
{
    ir_init();
    
    ir_export("test.vcg");
}