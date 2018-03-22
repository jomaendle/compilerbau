

%{
   #include <stdio.h>
   #include <stdlib.h>

   extern int yyerror(const char* err);
   extern int yylex(void);
   extern FILE *yyin;
%}

%locations

%define parse.error verbose

%start prog

%token IDENT
%token STRINGLIT
%token INTLIT
%token INTEGER STRING
%token IF WHILE RETURN PRINT ELSE
%token COMPARE NEQ LT GT LEQ GEQ
%left  PLUS MINUS
%left  MULT DIVIDE
%right UMINUS
%token OPENPAR CLOSEPAR
%token SEMI COM
%token OPENCURLY CLOSECURLY

%token ERROR


%%

prog: /* Nothing */
    | prog def
;

def: vardef
   | fundef
;

vardef: type idlist SEMI
;

idlist: IDENT
      | idlist COM IDENT

fundef: type IDENT OPENPAR params CLOSEPAR body
;

type: STRING
    | INTEGER
;

params: /* empty */
      | paramlist
;

paramlist: param
         | paramlist COM param
;

param: type IDENT
;

body: OPENCURLY vardefs stmts CLOSECURLY
;

vardefs: /* empty */
       | vardefs vardef
;


stmts: /* empty */
     | stmts stmt
;

stmt: while_stmt
    | if_stmt
    | ret_stmt
    | print_stmt
    | assign
    | funcall_stmt
;

while_stmt: WHILE OPENPAR boolexpr CLOSEPAR body
;

if_stmt: IF OPENPAR boolexpr CLOSEPAR body
       | IF OPENPAR boolexpr CLOSEPAR body ELSE body
;

ret_stmt: RETURN expr SEMI
;

print_stmt: PRINT expr SEMI
;

assign: IDENT COMPARE expr SEMI
;

funcall_stmt: funcall SEMI
;

boolexpr: expr COMPARE expr
        | expr NEQ expr
        | expr LT expr
        | expr GT expr
        | expr LEQ expr
        | expr GEQ expr
;

expr: funcall
    | INTLIT
    | IDENT
    | STRINGLIT
    | OPENPAR expr CLOSEPAR
    | expr PLUS expr
    | expr MINUS expr
    | expr MULT expr
    | expr DIVIDE expr
    | MINUS expr %prec UMINUS
;

funcall: IDENT OPENPAR args CLOSEPAR
;

args: /* empty */
    | arglist
;

arglist: expr
       | arglist COM expr
;


%%

int yyerror(const char* err){
  printf("Error: %s\n", err);
  return 0;
}


int main( int argc, char **argv )
{
    ++argv, --argc;
    if(argc>0){
      yyin = fopen(argv[0], "r");}
      else{
          yyin = stdin;
      }
      
     return yyparse();
}
