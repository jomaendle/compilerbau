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

   extern int yyerror(const char* err);
   extern int yylex(void);
   extern FILE *yyin;

   AST_p ast;

%}

%locations
%define parse.error verbose
%define api.value.type {AST_p}


%start start

%token IDENT
%token STRINGLIT
%token INTLIT
%left  PLUS MINUS
%left  MULT DIV
%right UMINUS
%token OPENPAR CLOSEPAR

%token ERROR


%%

start: expr {$$=$1; ast=$1;}

expr: INTLIT  {$$ = $1;}
    | IDENT   {$$ = $1;}
    | STRINGLIT {$$ = $1;}
    | OPENPAR expr CLOSEPAR {$$ = $2;ASTFree($1);ASTFree($3);}
    | expr PLUS expr  {$$=ASTAlloc2(t_PLUS, NULL, 0, $1, $3);ASTFree($2);}
    | expr MINUS expr {$$=ASTAlloc2(t_MINUS, NULL, 0, $1, $3);ASTFree($2);}
    | expr MULT expr  {$$=ASTAlloc2(t_MULT, NULL, 0, $1, $3);ASTFree($2);}
    | expr DIV expr   {$$=ASTAlloc2(t_DIV, NULL, 0, $1, $3);ASTFree($2);}
    | MINUS expr %prec UMINUS {$$=ASTAlloc2(t_MINUS, NULL, 0, $2, NULL);ASTFree($1);}
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
