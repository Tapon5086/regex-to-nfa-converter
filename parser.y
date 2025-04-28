%{
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

/* Define this here and then it gets used in the actual NFA code */
struct NFA* result_nfa;
%}

%union {
    char character;
    struct NFA* nfa;
}

%token <character> CHAR ESCAPED
%token LPAREN RPAREN OR STAR PLUS QMARK DOT LBRACKET RBRACKET DASH

%type <nfa> regex term factor atom char_class char_range

%%

regex
    : term                       { $$ = $1; result_nfa = $$; }
    | regex OR term              { $$ = create_union_nfa($1, $3); result_nfa = $$; }
    ;

term
    : factor                     { $$ = $1; }
    | term factor                { $$ = create_concat_nfa($1, $2); }
    ;

factor
    : atom                       { $$ = $1; }
    | atom STAR                  { $$ = create_kleene_star_nfa($1); }
    | atom PLUS                  { $$ = create_plus_nfa($1); }
    | atom QMARK                 { $$ = create_question_nfa($1); }
    ;

atom
    : CHAR                       { $$ = create_char_nfa($1); }
    | ESCAPED                    { $$ = create_char_nfa($1); }
    | DOT                        { $$ = create_any_char_nfa(); }
    | LPAREN regex RPAREN        { $$ = $2; }
    | char_class                 { $$ = $1; }
    ;

char_class
    : LBRACKET char_range RBRACKET { $$ = $2; }
    ;

char_range
    : CHAR                       { $$ = create_char_nfa($1); }
    | ESCAPED                    { $$ = create_char_nfa($1); }
    | CHAR DASH CHAR             { $$ = create_char_range_nfa($1, $3); }
    | char_range char_range      { $$ = create_union_nfa($1, $2); }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
    exit(1);
}