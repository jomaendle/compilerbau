#include "ast.h"


long nodectr=0;

char* ast_name[] =
{
   "nil",
   "prog",
   "def",
   "vardef",
   "fundef",
   "type",
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
   "t_WHILE",
   "t_RETURN",
   "t_PRINT",
   "t_dummy",
   "t_ELSE",
   "idlist"
};


static void dot_print_child(FILE* out, AST_p father, AST_p child)
{
   if(child)
   {
      fprintf(out, "   ast%ld -> ast%ld\n", father->nodectr, child->nodectr);
      DOTASTNodePrint(out, child);
   }
}

void ASTCellPrint(AST_p cell)
{
   printf("ASTCell(%d, %s, %s, %ld, %p, %p, %p, %p)\n",
          cell->type,
          ast_name[cell->type],
          cell->litval,
          cell->intval,
          cell->child[0],
          cell->child[1],
          cell->child[2],
          cell->child[3]);
}

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

AST_p ASTAlloc(ASTNodeType type, char* litval, long intval, AST_p child0,
               AST_p child1, AST_p child2, AST_p child3)
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
   /* ASTCellPrint(ast); */

   return ast;
}

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

void DOTASTPrint(FILE* out, AST_p ast)
{
   fprintf(out, "digraph ast {\n   ordering=out\n");
   DOTASTNodePrint(out, ast);
   fprintf(out, "}\n");
}

void ExprASTPrint(FILE* out, AST_p ast)
{
   switch(ast->type)
   {
   case nil:
         break;
   case prog:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   // case def:
   //       break;
   case vardef:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case fundef:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[2]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[3]);
         fprintf(out, ")");
         break;
   case type:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, ")");
         break;
   case params:
         fprintf(out, "(");
         if(ast->child[0]) {
              ExprASTPrint(out, ast->child[0]);
         }
         fprintf(out, ")");
         break;
   case paramlist:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case param:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case body:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case vardefs:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case stmts:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   // case stmt:
   //       break;
   case while_stmt:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[2]);
         fprintf(out, ")");
         break;
   case if_stmt:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[2]);
         if(ast->child[3]) {
           fprintf(out, "else ");
           ExprASTPrint(out, ast->child[3]);
         }
         fprintf(out, ")");
         break;
   // case else_stmt:
   //       break;
   case ret_stmt:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case print_stmt:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case assign:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " = ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case funcall_stmt:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, ")");
         break;
   // case expr:
   //       break;
   case funcall:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   // case args:
   //       break;
   case arglist:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, " ");
         if(ast->child[1]) {
              ExprASTPrint(out, ast->child[1]);
         }
         fprintf(out, ")");
         break;
   // case arg:
   //       break;
   case t_STRINGLIT:
         fprintf(out, "%s", ast->litval);
         break;
   case t_IDENT:
         fprintf(out, "%s", ast->litval);
         break;
   case t_INTLIT:
         fprintf(out, "%s", ast->litval);
         break;
   case t_PLUS:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "+");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_MULT:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "*");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_DIV:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "/");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_MINUS:
         if(ast->child[1]) /* Binary minus */
         {
            fprintf(out, "(");
            ExprASTPrint(out, ast->child[0]);
            fprintf(out, "-");
            ExprASTPrint(out, ast->child[1]);
            fprintf(out, ")");
         }
         else /* Unary minus */
         {
            fprintf(out, "-");
            ExprASTPrint(out, ast->child[0]);
         }
         break;
   case t_EQ:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "=");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_NEQ:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "!=");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_LT:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "<");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_GT:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, ">");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_LEQ:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "<=");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_GEQ:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, ">=");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_INTEGER:
         fprintf(out, "%s", ast->litval);
         break;
   case t_STRING:
         fprintf(out, "%s", ast->litval);
         break;
   case t_IF:
         fprintf(out, "%s", ast->litval);
         break;
   case t_WHILE:
         fprintf(out, "%s", ast->litval);
         break;
   case t_RETURN:
         fprintf(out, "%s", ast->litval);
         break;
   case t_PRINT:
         fprintf(out, "%s", ast->litval);
         break;
   // case t_ELSE:
   //       break;
   case idlist:
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, ", ");
         ExprASTPrint(out, ast->child[1]);
         break;

   default:
         fprintf(out, "Unexpected construct: %s\n", ast_name[ast->type]);
         break;
   }
}

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
