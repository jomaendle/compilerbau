/*
    Scanner for the nanoLang compiler.
    Authors: Konstantin Kläger, Isabel Staaden, Carolin Rösch
*/

%option noyywrap yylineno

%{
  # include <string.h>
  # include "ast.h"
  # include "nanoLangParser.tab.h"


  int yycolno = 1;
  void yyerror(char *err, ...);

  #define YY_USER_ACTION {\
      yylval=ASTEmptyAlloc();\
      yylval->litval = strdup(yytext);\
   }
%}


%%


#[^\n]* {  /* Skip comments */  }
"Integer" { yylval->type = t_INTEGER; return INTEGER; }
"String" { yylval->type = t_STRING; return STRING; }
"print" { yylval->type = t_PRINT; return PRINT; }
"return" { yylval->type = t_RETURN; return RETURN; }
"if" { yylval->type = t_IF; return IF; }
"else" { yylval->type = t_ELSE; return ELSE; }
"while" { yylval->type = t_WHILE; return WHILE; }
[0-9]+ { yylval->type = t_INTLIT; yylval->intval = atol(yytext); return INTLIT; }
\"[^"]*\" { yylval->type = t_STRINGLIT; return STRINGLIT; }
[A-Za-z]([A-Za-z0-9])* { yylval->type = t_IDENT; return IDENT; }
"(" { return OPENPAR; }
")" { return CLOSEPAR; }
"{" { return OPENCURLY; }
"}" { return CLOSECURLY; }
";" { return SEMICOLON; }
"," { return COMMA; }
"*"  { yylval->type = t_MULT; return MULT; }
"/"  { yylval->type = t_DIV; return DIV; }
"+"  { yylval->type = t_PLUS; return PLUS; }
"-"  { yylval->type = t_MINUS; return MINUS; }
"<" { yylval->type = t_LT; return LT; }
"<=" { yylval->type = t_LEQ; return LEQ; }
">" { yylval->type = t_GT; return GT; }
">=" { yylval->type = t_GEQ; return GEQ; }
"!=" { yylval->type = t_NEQ; return NEQ; }
"=" { yylval->type = t_EQ; return EQ; }
"\n" { yycolno = 1; }
[\t\r ] { /* Skip  whitespace*/ }
. { yyerror("Unexpected charater: %c", *yytext); }


%%
