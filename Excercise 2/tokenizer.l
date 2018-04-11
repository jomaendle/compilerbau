%option noyywrap
%option yylineno

%{
   #include <string.h>
   #include "ast.h"
   #include "nanoparser.tab.h"

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
"="  {yylval->type = t_EQ;return EQ;}
"!=" {yylval->type = t_NEQ;return NEQ;}
">"  {yylval->type = t_GT;return GT;}
"<"  {yylval->type = t_LT;return LT;}
">=" {yylval->type = t_GEQ;return GEQ;}
"<=" {yylval->type = t_LEQ;return LEQ;}
"{"  {return OPENCURLY;}
"}"  {return CLOSECURLY;}
";"  {return SEMICOLON;}
","  {return COMA;}


Integer {yylval->type = t_INTEGER;return INTEGER;}
String  {yylval->type = t_STRING;return STRING;}
if      {return IF;}
while   {return WHILE;}
return  {return RETURN;}
print   {return PRINT;}

{ALPHA}{ALNUM}* {yylval->type = t_IDENT;return IDENT;}

\"[^"]*\"       {yylval-> type = t_STRINGLIT;return STRINGLIT;}
{DIGIT}+        {yylval->type = t_INTLIT;return INTLIT;}
#[^\n]* { /* Eat comments */;}
[\n]    {yycol = 0;}
[\r\t ] {/* Skip  whitespace*/ }
 /* Everything else is an invalid character. */
.   {return ERROR;}
%%
/*---------------------------------------------------------------------*/
/*                        Global Variables                             */
/*---------------------------------------------------------------------*/
/* Names of the tokens for nice output. */
char* tokname[] =
{
   "End Of File",
   "OPENPAR",
   "CLOSEPAR",
   "MULT",
   "DIV",
   "PLUS",
   "MINUS",
   "EQ",
   "NEQ",
   "LT",
   "GT",
   "LEQ",
   "GEQ",
   "OPENCURLY",
   "CLOSECURLY",
   "SEMICOLON",
   "COMA",
   "IDENT",
   "STRINGLIT",
   "INTLIT",
   "INTEGER",
   "STRING",
   "IF",
   "WHILE",
   "RETURN",
   "PRINT",
   "ERROR"
};
/* yylval is the semantic value, used by the parser, but set by the
 * lexer. The variable is provided by the parser side. */
YYSTYPE yylval;
