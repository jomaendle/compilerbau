 /*
     nanoLang expression parser

     Copyright 2015-2017 by Stephan Schulz, schulz@eprover.org.

     This code is released under the GNU General Public Licence
     Version 2.
 */

%{
   #include <stdio.h>
   #include <string.h>
   #include "ast.h"
   #include "types.h"
   #include "semantic.h"

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
%token EQ NEQ LT GT LEQ GEQ
%left  PLUS MINUS
%left  MULT DIV
%right UMINUS
%token OPENPAR CLOSEPAR
%token SEMICOLON COMA
%token OPENCURLY CLOSECURLY

%token ERROR


%%



prog: /* Nothing */ {$$=NULL, ast=$$;}
    | prog def {
       if($1){$$ = ASTAlloc2(prog, @1.first_line, @1.first_column, NULL, 0, $1, $2);}
       else{$$=$2;} ast=$$;}
    | prog error {$$=NULL; ASTFree($1);}
;

def: vardef {$$=$1;}
   | fundef {$$=$1;}
;

vardef: type idlist SEMICOLON {
   $$ = ASTAlloc2(vardef, @1.first_line, @1.first_column, NULL, 0, $1, $2);ASTFree($3);}

;

idlist: IDENT {$$=$1;}
      | idlist COMA IDENT {
         $$ = ASTAlloc2(idlist, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}


fundef: type IDENT OPENPAR params CLOSEPAR body {
   $$ = ASTAlloc(fundef, @1.first_line, @1.first_column, NULL, 0, $1, $2, $4, $6);ASTFree($3);ASTFree($5);}

;

type: STRING  {$$ = $1;}
    | INTEGER {$$ = $1;}
;

params: /* empty */ {$$ = ASTAlloc2(params, @$.first_line, @$.first_column, NULL,0,NULL,NULL);}
      | paramlist {$$ = ASTAlloc2(params,  @1.first_line, @1.first_column, NULL,0,$1,NULL);}
;

paramlist: param {$$ = $1;}
      | paramlist COMA param {$$ = ASTAlloc2(paramlist, @1.first_line, @1.first_column, NULL,0,$1,$3);
                              ASTFree($2);}
;

param: type IDENT {$$ = ASTAlloc2(param, @1.first_line, @1.first_column, NULL, 0, $1, $2);}
;

body: OPENCURLY vardefs stmts CLOSECURLY{
   if($2){$$ = ASTAlloc2(body, @1.first_line, @1.first_column, NULL, 0, $2, $3);}
   else{$$=ASTAlloc2(body, @1.first_line, @1.first_column, NULL, 0, $3, NULL);}
   ASTFree($1);ASTFree($4);}
;

vardefs: /* empty */ {$$=NULL;}
    | vardefs vardef {
       if($1){$$=ASTAlloc2(vardefs, @1.first_line, @1.first_column, NULL, 0, $1, $2);}
       else{$$=$2;}}
;


stmts: /* empty */ {$$=NULL;}
     | stmts stmt {
        if($1){$$=ASTAlloc2(stmts, @1.first_line, @1.first_column, NULL, 0, $1, $2);}
        else{$$=ASTAlloc2(stmt, @1.first_line, @1.first_column, NULL, 0, $2, NULL);}}
;

stmt: while_stmt    {$$ = $1;}
    | if_stmt       {$$ = $1;}
    | ret_stmt      {$$ = $1;}
    | print_stmt    {$$ = $1;}
    | assign        {$$ = $1;}
    | funcall_stmt  {$$ = $1;}
;

while_stmt: WHILE OPENPAR boolexpr CLOSEPAR body {
   $$=ASTAlloc2(while_stmt, @1.first_line, @1.first_column, NULL, 0, $3, $5);
   ASTFree($1);
   ASTFree($2);
   ASTFree($4);
 }
;

if_stmt: IF OPENPAR boolexpr CLOSEPAR body {
        $$=ASTAlloc2(if_stmt, @1.first_line, @1.first_column, NULL, 0, $3, $5);
        ASTFree($1);
        ASTFree($2);
        ASTFree($4);
     }
   | IF OPENPAR boolexpr CLOSEPAR body ELSE body{
      $$=ASTAlloc(if_stmt, @1.first_line, @1.first_column, NULL, 0, $3, $5, $7, NULL);
      ASTFree($1);
      ASTFree($2);
      ASTFree($4);
      ASTFree($6);
     }
;

ret_stmt: RETURN expr SEMICOLON {$$=ASTAlloc2(ret_stmt, @1.first_line, @1.first_column, NULL, 0, $2, NULL);
                                 ASTFree($1); ASTFree($3);}
;

print_stmt: PRINT expr SEMICOLON {$$=ASTAlloc2(print_stmt, @1.first_line, @1.first_column, NULL, 0, $2, NULL);
                                 ASTFree($1); ASTFree($3);}
;

assign: IDENT EQ expr SEMICOLON  {$$=ASTAlloc2(assign, @1.first_line, @1.first_column, NULL, 0, $1, $3);
    ASTFree($2); ASTFree($4);}
;

funcall_stmt: funcall SEMICOLON {$$=$1; ASTFree($2);}
;

boolexpr: expr EQ expr {$$=ASTAlloc2(t_EQ, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
        | expr NEQ expr{$$=ASTAlloc2(t_NEQ, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
        | expr LT expr {$$=ASTAlloc2(t_LT, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
        | expr GT expr {$$=ASTAlloc2(t_GT, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
        | expr LEQ expr{$$=ASTAlloc2(t_LEQ, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
        | expr GEQ expr{$$=ASTAlloc2(t_GEQ, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
;

expr: funcall   {$$ = $1;}
    | INTLIT    {$$ = $1;}
    | IDENT     {$$ = $1;}
    | STRINGLIT {$$ = $1;}
    | OPENPAR expr CLOSEPAR {$$ = $2;ASTFree($1);ASTFree($3);}
    | expr PLUS expr  {$$=ASTAlloc2(t_PLUS, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
    | expr MINUS expr {$$=ASTAlloc2(t_MINUS, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
    | expr MULT expr  {$$=ASTAlloc2(t_MULT, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
    | expr DIV expr   {$$=ASTAlloc2(t_DIV, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);}
    | MINUS expr %prec UMINUS {$$=ASTAlloc2(t_MINUS, @1.first_line, @1.first_column, NULL, 0, $2, NULL);ASTFree($1);}
;

funcall: IDENT OPENPAR args CLOSEPAR {
   $$=ASTAlloc2(funcall, @1.first_line, @1.first_column, NULL, 0, $1, $3); ASTFree($2);ASTFree($4);
 }
;

args: /* empty */{$$=NULL;}
    | arglist {$$ = $1;}

;

arglist: expr {$$ = $1;}
       | arglist COMA expr {
          $$=ASTAlloc2(arglist, @1.first_line, @1.first_column, NULL, 0, $1, $3);ASTFree($2);
         }
;


%%


int yyerror(const char* err)
{
   printf("%d:%d Error: %s\n", yylloc.first_line, yylloc.first_column, err);
   return 0;
}


int main (int argc, char* argv[])
{
  int i;
  int res;
  bool printdot   = false;
  bool printsexpr = false;

   ++argv, --argc;  /* skip over program name */

   if (argc > 0)
   {
      if(strcmp(argv[0], "--dot")==0)
      {
         printdot   = true;
         printsexpr = false;
         ++argv, --argc;
      }
      else if(strcmp(argv[0], "--sexpr")==0)
      {
         printdot   = false;
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

   if(res==0)
   {
      bool no_errors;

      TypeTable_p   tt = TypeTableAlloc();
      SymbolTable_p st = SymbolTableAlloc();

      no_errors = STBuildAllTables(st, tt, ast);
      fprintf(stdout,"Global symbols:\n---------------\n");
      SymbolTablePrintLocal(stdout, st, tt);
      fprintf(stdout,"\nTypes:\n------\n");
      TypeTablePrint(stdout, tt);

      if(printdot)
      {
         DOTASTPrint(stdout, ast);
      }
      if(printsexpr)
      {
         SExprASTPrint(stdout, ast);
         printf("\n");
      }
   }
   return res;
}
