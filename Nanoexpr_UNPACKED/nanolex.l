 /* -*- Mode: c -*-

     Lexer for a minimal programming language "nanoLang"

     Copyright 2015-2017 by Stephan Schulz, schulz@eprover.org.

     This code is released under the GNU General Public Licence
     Version 2.
 */

%option noyywrap
%option yylineno

%{
   #include <string.h>
   #include "ast.h"
   #include "nanoparse.tab.h"

   int yycol = 1;

#define YY_USER_ACTION {\
      yylval=ASTEmptyAlloc();\
      yylval->litval = strdup(yytext);\
      \
      yylloc.first_line = yylloc.last_line =yylineno;   \
      yylloc.first_column = yycol; \
      yycol += yyleng;\
      yylloc.last_column = yycol-1; \
   }

%}

DIGIT      [0-9]
ALPHA      [a-zA-Z]
ALNUM      {ALPHA}|{DIGIT}

INT        {DIGIT}+
ID         {ALPHA}{ALNUM}*


%%

"("  {return OPENPAR;}
")"  {return CLOSEPAR;}
"*"  {yylval->type = t_MULT; return MULT;}
"/"  {yylval->type = t_DIV; return DIV;}
"+"  {yylval->type = t_PLUS; return PLUS;}
"-"  {yylval->type = t_MINUS; return MINUS;}


{ALPHA}{ALNUM}* {yylval->type = t_IDENT; return IDENT;}

\"[^"]*\"       {yylval->type = t_STRINGLIT; return STRINGLIT;}

{DIGIT}+        {yylval->type = t_INTLIT; yylval->intval = atol(yytext); return INTLIT;}


#[^\n]* { /* Eat comments */}

[\n]    {yycol=1;}

[\r\t ] { /* Skip  whitespace*/ }

 /* Everything else is an invalid character. */
.   {return ERROR;}

%%
