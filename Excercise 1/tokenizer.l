/* Find and add all integers and floats... */

%option noyywrap

%{
 int rowCounter = 1;
 int coloumnCounter = 1;
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
  printf("%4d, %4d : Integer Statement=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{STRING} {
  printf("%4d, %4d : String Statement=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{PRINT} {
  printf("%4d, %4d : print Statement=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{RETURN} {
  printf("%4d, %4d : return Statement=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{WHILE} {
  printf("%4d, %4d : while Statement=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{ELSE} {
  printf("%4d, %4d : else Statement=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{IF} {
  printf("%4d, %4d : if Statement=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{IDENT} {
  printf("%4d, %4d : Identifier=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{INTLIT} {
  printf("%4d, %4d : INTEGER=\"%d\"\n", rowCounter, coloumnCounter, atoi(yytext));
  coloumnCounter += yyleng;
}

{STRINGLIT} {
  printf("%4d, %4d : STRING=\"%s\"\n",rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}


{OPENPAR} {
  printf("%4d, %4d : OPENPAR=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{CLOSEPAR} {
  printf("%4d, %4d : CLOSEPAR=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{OPENCURLY} {
  printf("%4d, %4d : OPENCURLY=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{CLOSECURLY} {
  printf("%4d, %4d : CLOSECURLY=\" %s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}



{PLUS} {
  printf("%4d, %4d : PLUS=\" %s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{MINUS} {
  printf("%4d, %4d : MINUS=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{DIVIDE} {
  printf("%4d, %4d : DIVIDE=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{MULT} {
  printf("%4d, %4d : MULT=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{COMPARE} {
  printf("%4d, %4d : ASSIGN=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{GT} {
  printf("%4d, %4d : GREATER THAN: %s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{LT} {
  printf("%4d, %4d : LESS THAN: %s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{NEQ} {
  printf("%4d, %4d : NEQ: %s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{COM} {
  printf("%4d, %4d : COMA=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{SEMI} {
  printf("%4d, %4d : SEMI=\"%s\"\n", rowCounter, coloumnCounter, yytext);
  coloumnCounter += yyleng;
}

{SPACE} {
  coloumnCounter++;
}

{NEWLINE} {
  rowCounter++;
  coloumnCounter = 1;
}



. {
    coloumnCounter++;
      /* Skip */
    }
%%

int main( int argc, char **argv )
{
 ++argv, --argc;  /* skip over program name */
 if ( argc > 0 )
    yyin = fopen( argv[0], "r" );
 else
    yyin = stdin;

 yylex();

}
