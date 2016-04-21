#include "print.h"


void print_function(Function* fun, unsigned int flags)
{
    printf("Function %s:", fun->name);
    // print params
    Variable* param;
    llist_foreach(VarList, llist_reverse(fun->params), param)
    {
        printf("  %s", param->name);
        switch(param->type)
        {
        case vt_label: printf("(label)");
                break;
        case vt_outint: printf("(inout)");
                break;
        case vt_int:
        default:
            break;
        }
    }
    printf("\n");
    
    if(flags & pf_labels)
    {
        printf("-Labels: ");
        Label* l;
        llist_foreach(LabelList, fun->labels, l)
        {
            printf("%s ", l->name);
        }
        printf("\n");
    }
    
    if(flags & pf_statments)
    {
        printf("-Statements:\n");
        Statement* s;
        llist_foreach(StatementList, llist_reverse(fun->statms), s)
        {
            printf("    %s", s->funname);
            if(s->fun == NULL)
            {
                printf("(unresolved)");
            }
            
            Argument a;
            llist_foreach(ArgList, llist_reverse(s->args), a)
            {
                printf("  ");
                switch(a.type)
                {
                    case at_iconst: printf("%i", a.arg.iconst); break;
                    case at_var: printf("%s", a.arg.var->name); break;
                    case at_label: printf("%s(label)", a.arg.label->name); break;
                    case at_var_or_label: printf("%s(unresolved)", a.arg.refname); break;
                }
            }
            
            printf("\n");
        }
    }
    
    if(flags & pf_recfunc)
    {
        printf("\n\n");
        Function* f;
        llist_foreach(FunList, fun->funcs, f)
        {
            print_function(f, flags);
        }
    }
}