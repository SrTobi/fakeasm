#ifndef _FAKEASM_MAIN_H
#define _FAKEASM_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int yylex(void);
int yyparse(void);
FILE *yyin;

void yyerrerf(const char*s, ...);
void yyerror(const char *s);


#include "ast.h"

void ast_analyze(Function* fun);

#endif