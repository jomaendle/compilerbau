/* Find and add all integers and floats... */

%option noyywrap


%{
 int    intval   = 0;
 double floatval = 0.0;
%}

DIGIT    [0-9]


%%



{DIGIT}+    {
 printf( "int:   %d (\"%s\")\n", atoi(yytext), yytext );
          intval += atoi(yytext);
          }

{DIGIT}+"."{DIGIT}*        {
          printf( "float: %f (\"%s\")\n", atof(yytext),yytext );
          floatval += atof(yytext);
          }

reset {
      intval = 0;
      floatval = 0;
      printf("Reset\n");
    }

print {
       printf("Current: %d : %f\n", intval, floatval);
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
