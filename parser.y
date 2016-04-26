%{
    #include "main.h"
	#include "parse.h"
%}

%union {
	int ival;
	char *sval;
	Variable* variable;
	VarList	varlist;
	ArgArray argarray;
	Argument argument;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> ICONST
%token <sval> ID FUNCID
%token EOL COMMA COLON AT RET DEF LB RB LBE RBE


%%
program:
	fbody
	;

fbody:
	statements
	;
	
statements:
	statements statement
	|
	;

statement:
	funcall
	| label
	| fundef
	| EOL
	;
	
funcall:
	funcid arglist EOL	{ push_funcall($1, $2); }
	;

%type <argarray> arglist args;
arglist:
	args
	|		{ $$ = llist_empty; }
	;

args:
	args COMMA arg		{ array_push($1, Argument, $3); $$ = $1; }
	| arg				{ $$ = array_new(Argument, 1); $$[0] = $1; }
	;

%type <argument> arg expr atomexpr;
arg:
	expr	
	| atomexpr
	;

expr:
	LB funcid arg RB				{ $$ = create_unary_arg($2, $3); }
	| LBE arg funcid arg RBE		{ $$ = create_binary_arg($3, $2, $4); }
	;

atomexpr:
	ICONST				{ $$ = create_iconst_arg($1); }
	| ID				{ $$ = create_label_or_var_arg($1); }
	| RET				{ $$ = create_label_or_var_arg("ret"); }
	;

label:
	ID COLON	{ push_label($1); }
	;

fundef:
	DEF funcid COLON
	paramlist EOL	{ push_function($2, $4);}
	fbody
	RET EOL			{ pop_function(); }
	;

%type <varlist> paramlist params;
paramlist:
	params
	|		{ $$ = llist_empty; }
	;

params:
	params COMMA param	{ $$ = llist_prepend($3, $1); }
	| param				{ $$ = llist_new(VarList, $1); }
	;

%type <variable> param;
param:
	COLON ID	{ $$ = create_parameter($2, vt_label); }
	| AT ID		{ $$ = create_parameter($2, vt_outint); }
	| ID		{ $$ = create_variable($1); }
	;	

%type <sval> funcid;
funcid:
	FUNCID
	| ID
	;
%%