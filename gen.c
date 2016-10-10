#include "gen.h"
#include <libfirm/firm.h>

void build_firm_function(Function* fun)
{
    fun->
    ir_type *method_type = new_type_method(0, 1, false, cc_cdecl_set, mtp_no_property);

	ir_type *type_int = new_type_primitive(mode_Is);
	set_method_res_type(method_type, 0, type_int);

	/* create an entity for the method in the global namespace */
	ident     *id          = new_id_from_str(fun->name);
	ir_type   *global_type = get_glob_type();
	ir_entity *entity      = new_entity(global_type, id, method_type);

	/* now we can create a new graph */
	ir_graph *irg = new_ir_graph(entity, MAIN_LOCAL_VARS);

	/* the function should be visible to the linker, and use symbol "main"
	 * in linker */
	set_entity_ld_ident(entity, id);


}


void build_firm_graph(Function* fun)
{
    ir_init();



    
    ir_export("test.vcg");
}