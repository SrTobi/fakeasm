%{
    #include "main.h"
    #include "parser.h"
    #define YY_DECL int yylex()
    
    static bool myended = false;
%}

%option noyywrap

DIGIT       [0-9]
ALPHA       [a-zA-Z]
ALPHANUM    [a-zA-Z0-9]
SIGN        [=<>/\\\.!?^~\-|$+*%'&]

%%
[ \t]               ;
"ret"               { return RET; }
"def"               { return DEF; }
":"                 { return COLON; }
"@"                 { return AT; }
","                 { return COMMA; }
"("                 { return LB; }
")"                 { return RB; }
"["                 { return LBE; }
"]"                 { return RBE; }
\n                  { return EOL; }
<<EOF>>             { bool end = myended; myended = true; return (end? YY_NULL : EOL); }
{DIGIT}+            { yylval.ival = atoi(yytext); return ICONST; }
";".*               { };

{ALPHA}{ALPHANUM}*  { yylval.sval = strdup(yytext); return ID; }
({SIGN}|{ALPHA})({ALPHANUM}|{SIGN})* { yylval.sval = strdup(yytext); return FUNCID; }
.                   { yyerror("Unknown char"); }
%%