#include "main.h"
#include "parse.h"
#include "print.h"


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

    print_function(root, pf_all);
    
    return 0;
}

void yyerror(const char *s) {
	printf("EEK, parse error!  Message: %s\n", s);
	// might as well halt now:
	exit(-1);
}