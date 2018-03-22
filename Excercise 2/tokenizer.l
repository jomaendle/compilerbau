/* Find and add all integers and floats... */

%option noyywrap

%option yylineno

%{
 int rowCounter = 1;
 int coloumnCounter = 1;
%}

%{
#include <string.h>
#include "parser.tab.h"


int yycol = 1;

/*
void print_tok(int tok)
{
   printf("<%5d:%3d: %d=\"%s\">\n", yylineno, yycol-yyleng, tok, yytext);
}
*/
#define YY_USER_ACTION {yycol += yyleng;}


%}

DIGIT      [0-9]
INT        {DIGIT}+
EXP        [eE](\+|-)?{INT}
REG        R{DIGIT}{DIGIT}?
CHAR       [a-zA-Z]
OPENPAR    "("
CLOSEPAR   ")"
OPENCURLY  "{"
CLOSECURLY "}"
PLUS       "+"
MINUS      "-"
DIVIDE     "/"
MULT       "*"
COMPARE    "="
GT         ">"
LT         "<"
NEQ        "!="
GEQ        ">="
LEQ        "<="
COM        ","
SEMI       ";"
SPACE      [ ]
NEWLINE    \n
IF         "if"
ELSE       "else"
WHILE      "while"
RETURN     "return"
PRINT      "print"
INTEGER    "Integer"
STRING     "String"

IDENT      [A-Za-z]([A-Za-z0-9])*
INTLIT     [0-9]+
STRINGLIT  \"[^"]*\"


%%


#[^\n]* {

}

{INTEGER} {

  coloumnCounter += yyleng;
  return INTEGER;
}

{STRING} {

  coloumnCounter += yyleng;
  return STRING;
}

{PRINT} {

  coloumnCounter += yyleng;
  return PRINT;
}

{RETURN} {

  coloumnCounter += yyleng;
  return RETURN;
}

{WHILE} {

  coloumnCounter += yyleng;
  return WHILE;
}

{ELSE} {

  coloumnCounter += yyleng;
  return ELSE;
}

{IF} {

  coloumnCounter += yyleng;
  return IF;
}

{IDENT} {

  coloumnCounter += yyleng;
  return IDENT;

}

{INTLIT} {

  coloumnCounter += yyleng;
  return INTLIT;
}

{STRINGLIT} {

  coloumnCounter += yyleng;
  return INTLIT;
}


{OPENPAR} {

  coloumnCounter += yyleng;
  return OPENPAR;
}

{CLOSEPAR} {

  coloumnCounter += yyleng;
  return CLOSEPAR;
}

{OPENCURLY} {

  coloumnCounter += yyleng;
  return OPENCURLY;
}

{CLOSECURLY} {

  coloumnCounter += yyleng;
  return CLOSECURLY;
}

{LEQ} {

  coloumnCounter += yyleng;
  return LEQ;
}

{GEQ} {

  coloumnCounter += yyleng;
  return PLUS;
}

{PLUS} {

  coloumnCounter += yyleng;
  return PLUS;
}

{MINUS} {
  coloumnCounter += yyleng;
  return MINUS;
}

{DIVIDE} {
  coloumnCounter += yyleng;
  return DIVIDE;
}

{MULT} {
  coloumnCounter += yyleng;
  return MULT;
}

{COMPARE} {
  coloumnCounter += yyleng;
  return COMPARE;
}

{GT} {
  coloumnCounter += yyleng;
  return GT;
}

{LT} {
  coloumnCounter += yyleng;
  return LT;
}

{NEQ} {
  coloumnCounter += yyleng;
  return NEQ;
}

{COM} {
  coloumnCounter += yyleng;
  return COM;
}

{SEMI} {
  coloumnCounter += yyleng;
  return SEMI;
}

{SPACE} {
  coloumnCounter++;
}

{NEWLINE} {
  yycol = 1;
}



. {
    coloumnCounter++;
      /* Skip */
    }
%%
