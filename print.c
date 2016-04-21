#include "print.h"


void print_vars(VarList vars)
{
    Variable* var;
    llist_foreach(VarList, llist_reverse(vars), var)
    {
        printf("%s", var->name);
        switch(var->type)
        {
        case vt_label: printf("(label)");
                break;
        case vt_outint: printf("(out)");
                break;
        case vt_int:
        default:
            break;
        }
        
        if(llist_tail(_idx) != llist_empty)
            printf(",  ");
    }
}

void print_fullfuncname(Function* fun)
{
    printf("%s", fun->name);
    if(fun->context != NULL)
    {
        printf("::");
        print_fullfuncname(fun->context);
    }
}

void print_function(Function* fun, unsigned int flags)
{
    if(flags & pf_fullfuncname)
    {
        printf("Function [");
        print_fullfuncname(fun);
        printf("]: ");
    }else{
        printf("Function %s:", fun->name);
    }
    // print params
    print_vars(fun->params);
    printf("\n");
    
    if(flags & pf_labels)
    {
        printf("-Labels:    ");
        Label* l;
        llist_foreach(LabelList, fun->labels, l)
        {
            printf("%s ", l->name);
        }
        printf("\n");
    }
    
    if(flags & pf_variables)
    {
        printf("-Variables:  ");
        print_vars(fun->vars);
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
                    case at_labelref: printf("%s(labelref)", a.arg.var->name); break;
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