/*-----------------------------------------------------------------------

File  : ast.h

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Copyright 2015-2017 by the author.
  This code is released under the GNU General Public Licence.

Changes

<1> Tue Mar 31 16:25:43 CEST 2015
    New

-----------------------------------------------------------------------*/

#ifndef AST

#define AST

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


/*---------------------------------------------------------------------*/
/*                    Data type declarations                           */
/*---------------------------------------------------------------------*/

typedef enum
{
   nil,
   prog,
   def,
   vardef,
   fundef,
   type,
   params,
   paramlist,
   param,
   body,
   vardefs,
   stmts,
   stmt,
   while_stmt,
   if_stmt,
   else_stmt,
   ret_stmt,
   print_stmt,
   assign,
   funcall_stmt,
   boolexpr,
   expr,
   funcall,
   args,
   arglist,
   arg,
   t_MULT,
   t_DIV,
   t_PLUS,
   t_MINUS,
   t_EQ,
   t_NEQ,
   t_LT,
   t_GT,
   t_LEQ,
   t_GEQ,
   t_IDENT,
   t_STRINGLIT,
   t_INTLIT,
   t_INTEGER,
   t_STRING,
   t_IF,
   t_WHILE,
   t_RETURN,
   t_PRINT,
   t_dummy,
   t_ELSE,
   idlist
}ASTNodeType;

#define MAXCHILD 5

typedef struct astcell
{
   ASTNodeType type;
   long        nodectr;
   char*       litval;
   long        intval;
   struct astcell *child[MAXCHILD];
}ASTCell, *AST_p;



/*---------------------------------------------------------------------*/
/*                Exported Functions and Variables                     */
/*---------------------------------------------------------------------*/

extern long nodectr;

#define ASTCellAlloc()    (ASTCell*)malloc(sizeof(ASTCell))
#define ASTCellFree(junk) free(junk)

AST_p ASTEmptyAlloc(void);
AST_p  ASTAlloc(ASTNodeType type, char* litval, long intval,
                AST_p child0, AST_p child1, AST_p child2, AST_p child3);

#define ASTAlloc2(type, litval, intval, child0, child1) \
   ASTAlloc((type), (litval), (intval), (child0), (child1), NULL, NULL)

void   ASTFree(AST_p junk);

void   DOTASTNodePrint(FILE* out, AST_p ast);
void   DOTASTPrint(FILE* out, AST_p ast);

void   ExprASTPrint(FILE* out, AST_p ast);
void   SExprASTPrint(FILE* out, AST_p ast);


#endif

/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
