/*-----------------------------------------------------------------------

File  : ast.c

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Implementation of an abstract syntax tree type.

  Copyright 2015-2017 by the author.
  This code is released under the GNU General Public Licence.

Changes

<1> Tue Mar 31 18:39:18 CEST 2015
    New
<2> Mon Mar 28 23:37:01 CEST 2016
    Update, add ASTFree()

-----------------------------------------------------------------------*/

#include "ast.h"



/*---------------------------------------------------------------------*/
/*                        Global Variables                             */
/*---------------------------------------------------------------------*/

long nodectr=0;

char* ast_name[] =
{
   "nil",
   "prog",
   "def",
   "vardef",
   "fundef",
   "type",
   "idlist",
   "params",
   "paramlist",
   "param",
   "body",
   "vardefs",
   "stmts",
   "stmt",
   "while_stmt",
   "if_stmt",
   "else_stmt",
   "ret_stmt",
   "print_stmt",
   "assign",
   "funcall_stmt",
   "boolexpr",
   "expr",
   "funcall",
   "args",
   "arglist",
   "arg",
   "t_MULT",
   "t_DIV",
   "t_PLUS",
   "t_MINUS",
   "t_EQ",
   "t_NEQ",
   "t_LT",
   "t_GT",
   "t_LEQ",
   "t_GEQ",
   "t_IDENT",
   "t_STRINGLIT",
   "t_INTLIT",
   "t_INTEGER",
   "t_STRING",
   "t_IF",
   "t_ELSE",
   "t_WHILE",
   "t_RETURN",
   "t_PRINT",
   "t_dummy",
   "XXXX"
};

/*---------------------------------------------------------------------*/
/*                      Forward Declarations                           */
/*---------------------------------------------------------------------*/


/*---------------------------------------------------------------------*/
/*                         Internal Functions                          */
/*---------------------------------------------------------------------*/

/*-----------------------------------------------------------------------
//
// Function: dot_print_child()
//
//   Print a link from father node to child node, then print the
//   subtree corresponding to the child in DOT syntax.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

static void dot_print_child(FILE* out, AST_p father, AST_p child)
{
   if(child)
   {
      fprintf(out, "   ast%ld -> ast%ld\n", father->nodectr, child->nodectr);
      DOTASTNodePrint(out, child);
   }
}


/*---------------------------------------------------------------------*/
/*                         Exported Functions                          */
/*---------------------------------------------------------------------*/




/*-----------------------------------------------------------------------
//
// Function: ASTCellPrint()
//
//   Print an ASTCell to stdout (for debugging).
//
// Global Variables: Reads ast_name[]
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void ASTCellPrint(AST_p cell)
{
   printf("ASTCell(%d, %s, %s, %ld, %d, %d, %p, %p, %p, %p)\n",
          cell->type,
          ast_name[cell->type],
          cell->litval,
          cell->intval,
          cell->line,
          cell->column,
          cell->child[0],
          cell->child[1],
          cell->child[2],
          cell->child[3]);
}



/*-----------------------------------------------------------------------
//
// Function: ASTEmptyAlloc()
//
//   Allocate an empty ASTNode with no children.
//
// Global Variables:
//
// Side Effects    :
//
/----------------------------------------------------------------------*/

AST_p ASTEmptyAlloc(void)
{
   AST_p ast = ASTCellAlloc();
   int i;

   ast->type = nil;
   ast->litval = NULL;
   ast->nodectr = nodectr++;
   for(i=0; i<MAXCHILD; i++)
   {
      ast->child[i] = NULL;
   }
   return ast;
}


/*-----------------------------------------------------------------------
//
// Function: ASTAlloc()
//
//   Allocate a new ASTCell, initialize it with the given values, and
//   return a pointer to it.
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/

AST_p ASTAlloc(ASTNodeType type, int line, int column, char* litval,
               long intval, AST_p child0, AST_p child1, AST_p child2,
               AST_p child3)
{
   AST_p ast = ASTEmptyAlloc();

   ast->type = type;
   if(litval)
   {
      ast->litval = strdup(litval);
   }
   ast->intval = intval;
   ast->child[0] = child0;
   ast->child[1] = child1;
   ast->child[2] = child2;
   ast->child[3] = child3;
   ast->line     = line;
   ast->column   = column;
   /* ASTCellPrint(ast); */

   return ast;
}


/*-----------------------------------------------------------------------
//
// Function: ASTFree()
//
//   Free an AST.
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/

void ASTFree(AST_p junk)
{
   if(junk)
   {
      free(junk->litval);
      int i;

      for(i=0; junk->child[i]; i++)
      {
         ASTFree(junk->child[i]);
      }
   }
   ASTCellFree(junk);
}



/*-----------------------------------------------------------------------
//
// Function: DOTASTNodePrint()
//
//   Print an AST node and its children in DOT notation.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void DOTASTNodePrint(FILE* out, AST_p ast)
{
   int i;

   switch(ast->type)
   {
   case t_STRINGLIT:
         fprintf(out, "   ast%ld [label=%s]\n",
                 ast->nodectr,
                 //ast_name[ast->type],
                 ast->litval);
         break;
   case t_IDENT:
   case t_INTLIT:
         fprintf(out, "   ast%ld [label=\"%s\\n%s\"]\n",
                 ast->nodectr,
                 ast_name[ast->type],
                 ast->litval);
         break;
   default:
         fprintf(out, "   ast%ld [label=\"%s\"]\n",
                 ast->nodectr,
                 ast_name[ast->type]);
         break;
   }
   for(i=0; ast->child[i]; i++)
   {
      dot_print_child(out, ast, ast->child[i]);
   }
}

/*-----------------------------------------------------------------------
//
// Function: DOTASTPrint()
//
//   Print a complete AST as a DOT format graph.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void DOTASTPrint(FILE* out, AST_p ast)
{
   fprintf(out, "digraph ast {\n   ordering=out\n");
   DOTASTNodePrint(out, ast);
   fprintf(out, "}\n");
}


/*-----------------------------------------------------------------------
//
// Function: SExprASTPrint()
//
//   Print an AST representing a nanoLang construct as an
//   s-expression.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void SExprASTPrint(FILE* out, AST_p ast)
{
   int i;

   if(ast)
   {
      switch(ast->type)
      {
      case t_STRINGLIT:
            fprintf(out, "t_STRINGLIT<%s> ",
                    ast->litval);
            break;
      case t_IDENT:
            fprintf(out, "t_IDENT<%s> ",
                    ast->litval);
            break;
      case t_INTLIT:
            fprintf(out, "t_INTLIT<%s> ",
                    ast->litval);
            break;
      case t_STRING:
      case t_INTEGER:
            fprintf(out, "%s ", ast_name[ast->type]);
            break;
      default:
            fprintf(out, "(");
            fprintf(out, "%s ", ast_name[ast->type]);
            for(i=0; ast->child[i]; i++)
            {
               SExprASTPrint(out, ast->child[i]);
            }
            fprintf(out, ") ");
            break;
      }
   }
}


/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
