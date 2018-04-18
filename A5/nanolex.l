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
      yylloc.first_line = yylloc.last_line = yylineno;   \
      yylloc.first_column = yycol; \
      yycol += yyleng;\
      yylloc.last_column = yycol-1;\
      yylval->line = yylloc.first_line;           \
      yylval->column = yylloc.first_column;           \
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
"="  {yylval->type = t_EQ; return EQ;}
"!=" {yylval->type = t_NEQ; return NEQ;}
">"  {yylval->type = t_GT; return GT;}
"<"  {yylval->type = t_LT; return LT;}
">=" {yylval->type = t_GEQ; return GEQ;}
"<=" {yylval->type = t_LEQ; return LEQ;}
"{"  {return OPENCURLY;}
"}"  {return CLOSECURLY;}
";"  {return SEMICOLON;}
","  {return COMA;}

Integer {yylval->type = t_INTEGER; return INTEGER;}
String  {yylval->type = t_STRING; return STRING;}
if      {return IF;}
else    {return ELSE;}
while   {return WHILE;}
return  {return RETURN;}
print   {return PRINT;}

{ALPHA}{ALNUM}* {yylval->type = t_IDENT; return IDENT;}

\"[^"]*\"       {yylval->type = t_STRINGLIT; return STRINGLIT;}

{DIGIT}+        {yylval->type = t_INTLIT; yylval->intval = atol(yytext); return INTLIT;}


#[^\n]* { /* Eat comments */}

[\n]    {yycol=1;}

[\r\t ] { /* Skip  whitespace*/ }

 /* Everything else is an invalid character. */
.   {return ERROR;}

%%
