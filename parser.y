%{
    #include "main.h"
	#include "parse.h"
%}

%union {
	int ival;
	char *sval;
	Variable* variable;
	VarList	varlist;
	ArgList arglist;
	Argument argument;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> ICONST
%token <sval> ID
%token EOL COMMA COLON AT RET DEF STAR LB RB LBE RBE


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
	ID arglist EOL	{ push_funcall($1, $2); }
	;

%type <arglist> arglist args;
arglist:
	args
	|		{ $$ = llist_empty; }
	;

args:
	args COMMA arg		{ $$ = llist_prepend($3, $1); }
	| arg				{ $$ = llist_new(ArgList, $1); }
	;

%type <argument> arg expr atomexpr;
arg:
	expr	
	| atomexpr
	;

expr:
	LB ID arg RB				{ $$ = create_unary_arg($2, $3); }
	| LBE arg ID arg RBE		{ $$ = create_binary_arg($3, $2, $4); }
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
	DEF ID COLON
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
	AT ID		{ $$ = create_parameter($2, vt_label); }
	| STAR ID	{ $$ = create_parameter($2, vt_outint); }
	| ID		{ $$ = create_variable($1); }
	;	

%%