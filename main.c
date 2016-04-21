#include "main.h"
#include "parse.h"
#include "print.h"
#include "ast.h"
#include <stdarg.h>

int main(int argc, char** argv) {

    if(argc > 1) {
        // open a file handle to a particular file:
        FILE *myfile = fopen(argv[1], "r");
        // make sure it's valid:
        if (!myfile) {
            printf("I can't open %s!", argv[1]);
            return -1;
        }
        
        // set lex to read from it instead of defaulting to STDIN:
        yyin = myfile;
    }

    push_function("root", llist_empty);
	Function* root = cur_func();
    
	// lex through the input:
	do {
		yyparse();
	} while (!feof(yyin));

    ast_analyze(root);
    print_function(root, pf_all);
    
    return 0;
}

void yyerrerf(const char* format, ...)
{
    char buffer[512];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer,512,format, args);
    yyerror(buffer);
    va_end (args);
}

void yyerror(const char *s) {
	printf("EEK, parse error!  Message: %s\n", s);
	// might as well halt now:
	exit(-1);
}