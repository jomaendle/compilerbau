

%{
   #include <stdio.h>
   #include <stdlib.h>
   #include "ast.h"

   extern int yyerror(const char* err);
   extern int yylex(void);
   extern FILE *yyin;

   AST_p ast;
%}

%locations

%define parse.error verbose
%define api.value.type {AST_p}

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

prog:  {$$ = 0;}
    | prog def {$$ = 0;}
;

def: vardef {$$ = 0;}
   | fundef {$$ = 0;}
;

vardef: type idlist SEMI {$$ = 0;}
;

idlist: IDENT {$$ = 0;}
      | idlist COM IDENT {$$ = 0;}

fundef: type IDENT OPENPAR params CLOSEPAR body {$$ = 0;}
;

type: STRING {$$ = 0;}
    | INTEGER {$$ = 0;}
;

params:  {$$ = 0;}
      | paramlist {$$ = 0;}
;

paramlist: param {$$ = 0;}
         | paramlist COM param {$$ = 0;}
;

param: type IDENT {$$ = 0;}
;

body: OPENCURLY vardefs stmts CLOSECURLY {$$ = 0;}
;

vardefs:  {$$ = 0;}
       | vardefs vardef {$$ = 0;}
;


stmts:  {$$ = 0;}
     | stmts stmt {$$ = 0;}
;

stmt: while_stmt {$$ = 0;}
    | if_stmt {$$ = 0;}
    | ret_stmt {$$ = 0;}
    | print_stmt {$$ = 0;}
    | assign {$$ = 0;}
    | funcall_stmt {$$ = 0;}
;

while_stmt: WHILE OPENPAR boolexpr CLOSEPAR body {$$ = 0;}
;

if_stmt: IF OPENPAR boolexpr CLOSEPAR body {$$ = 0;}
       | IF OPENPAR boolexpr CLOSEPAR body ELSE body {$$ = 0;}
;

ret_stmt: RETURN expr SEMI {$$ = 0;}
;

print_stmt: PRINT expr SEMI {$$ = 0;}
;

assign: IDENT COMPARE expr SEMI {$$ = 0;}
;

funcall_stmt: funcall SEMI {$$ = 0;}
;

boolexpr: expr COMPARE expr  {$$ = 0;}
        | expr NEQ expr {$$ = 0;}
        | expr LT expr {$$ = 0;}
        | expr GT expr {$$ = 0;}
        | expr LEQ expr {$$ = 0;}
        | expr GEQ expr {$$ = 0;}
;

expr: funcall
    | INTLIT  {$$ = $1;}
    | IDENT   {$$ = $1;}
    | STRINGLIT {$$ = $1;}
    | OPENPAR expr CLOSEPAR {$$ = $2;ASTFree($1);ASTFree($3);}
    | expr PLUS expr  {$$=ASTAlloc2(t_PLUS, NULL, 0, $1, $3);ASTFree($2);}
    | expr MINUS expr {$$=ASTAlloc2(t_MINUS, NULL, 0, $1, $3);ASTFree($2);}
    | expr MULT expr  {$$=ASTAlloc2(t_MULT, NULL, 0, $1, $3);ASTFree($2);}
    | expr DIVIDE expr   {$$=ASTAlloc2(t_DIV, NULL, 0, $1, $3);ASTFree($2);}
    | MINUS expr %prec UMINUS {$$=ASTAlloc2(t_MINUS, NULL, 0, $2, NULL);ASTFree($1);}
;

funcall: IDENT OPENPAR args CLOSEPAR {$$ = 0;}
;

args:  {$$ = 0;}
    | arglist {$$ = 0;}
;

arglist: expr {$$ = 0;}
       | arglist COM expr {$$ = 0;}
;


%%

int yyerror(const char* err){
  printf("Error: %s\n", err);
  return 0;
}


int main (int argc, char* argv[])
{
  int i;
  int res;
  bool printdot   = false;
  bool printexpr  = true;
  bool printsexpr = false;

   ++argv, --argc;  /* skip over program name */

   if (argc > 0)
   {
      if(strcmp(argv[0], "--dot")==0)
      {
         printdot   = true;
         printexpr  = false;
         printsexpr = false;
         ++argv, --argc;
      }
      else if(strcmp(argv[0], "--sexpr")==0)
      {
         printdot   = false;
         printexpr  = false;
         printsexpr = true;
         ++argv, --argc;
      }
   }

   if ( argc > 0 )
   {
      yyin = fopen( argv[0], "r" );
   }
   else
   {
      yyin = stdin;
   }

   res = yyparse();

   if(res == 0)
   {
      if(printdot)
      {
         DOTASTPrint(stdout, ast);
      }
      if(printexpr)
      {
         ExprASTPrint(stdout, ast);
         printf("\n");
      }
      if(printsexpr)
      {
         SExprASTPrint(stdout, ast);
         printf("\n");
      }
      ASTFree(ast);
   }
   return res;
}
