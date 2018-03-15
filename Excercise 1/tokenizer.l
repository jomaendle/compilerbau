/* Find and add all integers and floats... */

%option noyywrap


%{
 int    intval   = 0;
 double floatval = 0.0;
%}

DIGIT      [0-9]
INT        {DIGIT}+
EXP        [eE](\+|-)?{INT}
REG        R{DIGIT}{DIGIT}?
CHAR       [a-zA-Z]
STRING     {CHAR}+
OPENPAR    [(]
CLOSEPAR   [)]
OPENCURLY  [{]
CLOSECURLY [}]
PLUS       [+]
MINUS      [-]
DIVIDE     [/]
MULT       [*]
COMPARE    [=]
GT         [>]
LT         [<]
NEQ        [!=]
COM        [,]
SEMI       [;]

INTLIT     [0-9]+
STRINGLIT  \"[^"]*\"

%%

#[^\n]* {
  /* eat comment */
}

{INTLIT} {
  printf("INTEGER=\"%d\n", atoi(yytext));
}

{STRINGLIT} {
      printf("STRING=\"%s\n", yytext);
    }
{STRING} {
  printf("STRING=\"%s\n", yytext);
}
{OPENPAR} {
      printf("OPENPAR=\"%s\n", yytext);
    }

{CLOSEPAR} {
      printf("CLOSEPAR=\"%s\n", yytext);
    }
{OPENCURLY} {
      printf("OPENCURLY=\"%s\n", yytext);
    }

{CLOSECURLY} {
      printf("CLOSECURLY=\" %s\n", yytext);
    }
{PLUS} {
  printf("PLUS=\" %s\n", yytext);
}
{MINUS} {
  printf("MINUS=\"%s\n", yytext);
}
{DIVIDE} {
  printf("DIVIDE=\"%s\n", yytext);
}
{MULT} {
  printf("MULT=\"%s\n", yytext);
}
{COMPARE} {
  printf("ASSIGN=\"%s\n", yytext);
}
{GT} {
  printf(">: %s\n", yytext);
}
{LT} {
  printf("<: %s\n", yytext);
}
{NEQ} {
  printf("NEQ: %s\n", yytext);
}
{COM} {
  printf("COMA=\"%s\n", yytext);
}
{SEMI} {
  printf("SEMI=\"%s\n", yytext);
}




\n|. {
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

 printf("Final  %d : %f\n", intval, floatval);
}
