 /*
     Minimal "scientific" calculator.

     The system reads and evaluates statements.
     Statements are either arithmetic expressions (over + and *) or
     assignements to one or 99 registers (R0-R99). Registers can be
     used in expressions. All numbers are interpreted as floating
     point.

     Example session:

     [Shell] ./scicalc
     R10=3*(5+4)
     > R10 = 27.000000
     (3.1415*R10+3)
     > 87.820500
     R9=(3.1415*R10+3)
     > R9 = 87.820496
     R9+R10
     > 114.820500
     ...

     Copyright 2014 by Stephan Schulz, schulz@eprover.org.

     This code is released under the GNU General Public Licence
     Version 2.
 */

%{
   #include <stdio.h>

   #define MAXREGS 100

   double regfile[MAXREGS];

   extern int yyerror(char* err);
   extern int yylex(void);
%}

%union {
   double val;
   int    regno;
}


%start stmtseq

%left  PLUS
%left  MULT
%left  DIVIDE
%left  MINUS
%token ASSIGN
%token OPENPAR
%token CLOSEPAR
%token NEWLINE
%token REGISTER
%token FLOAT
%token ERROR


%%

stmtseq: /* Empty */
    | NEWLINE stmtseq       {}
    | stmt  NEWLINE stmtseq {}
    | error NEWLINE stmtseq {};  /* After an error, start afresh */

stmt: assign  {printf("> RegVal: %f\n", $<val>1);}
     |expr    {printf("> %f\n", $<val>1);};

assign: REGISTER ASSIGN expr {regfile[$<regno>1] = $<val>3;
                              $<val>$ = $<val>3;} ;

expr: expr PLUS term {$<val>$ = $<val>1 + $<val>3;}
    | expr MINUS term {$<val>$ = $<val>1 - $<val>3;}
    | term {$<val>$ =  $<val>1;};

term: term MULT factor {$<val>$ = $<val>1 * $<val>3;}
    | term DIVIDE factor {$<val>$ = $<val>1 / $<val>3;}
    | factor {$<val>$ =  $<val>1;};

factor: REGISTER {$<val>$ = regfile[$<regno>1];}
      | FLOAT  {$<val>$ =  $<val>1;}
      | OPENPAR expr CLOSEPAR {$<val>$ =  $<val>2;};

%%

int yyerror(char* err)
{
   printf("Error: %s\n", err);
   return 0;
}


int main (int argc, char* argv[])
{
  int i;

  for(i=0; i<MAXREGS; i++)
  {
     regfile[i] = 0.0;
  }
  return yyparse();
}
