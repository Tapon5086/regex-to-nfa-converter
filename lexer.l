%{
#include <stdio.h>
#include "parser.tab.h"
%}

%%
"("             { return LPAREN; }
")"             { return RPAREN; }
"|"             { return OR; }
"*"             { return STAR; }
"+"             { return PLUS; }
"?"             { return QMARK; }
"."             { return DOT; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }
"-"             { return DASH; }
\\[nrt\\\(\)\|\*\+\?\.\[\]\-] { 
                  yylval.character = yytext[1]; 
                  return ESCAPED; 
                }
[a-zA-Z0-9]     { 
                  yylval.character = yytext[0]; 
                  return CHAR; 
                }
[ \t\n]         { /* Skip whitespace */ }
.               { 
                  fprintf(stderr, "Unrecognized character: %s\n", yytext); 
                }
%%

int yywrap(void) {
    return 1;
}