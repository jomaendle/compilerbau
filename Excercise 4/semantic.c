/*-----------------------------------------------------------------------

File  : semantic.h

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Semantic checks and annotations for nanoLang.

  Copyright 2015 by the author.
  This code is released under the GNU General Public Licence.

Changes

<1> Mon Apr 20 01:12:05 CEST 2015
    New

-----------------------------------------------------------------------*/

#include "semantic.h"



/*---------------------------------------------------------------------*/
/*                        Global Variables                             */
/*---------------------------------------------------------------------*/


/*---------------------------------------------------------------------*/
/*                      Forward Declarations                           */
/*---------------------------------------------------------------------*/


/*---------------------------------------------------------------------*/
/*                         Internal Functions                          */
/*---------------------------------------------------------------------*/






/*-----------------------------------------------------------------------*/
//
// Function: get_type_ast_type()
//
//   Given an AST node corresponding to a basic type, return the
//   corresponding type. Otherwise, exit with error.
//
// Global Variables: -
//
// Side Effects    : -
//
/*----------------------------------------------------------------------*/

static TypeIndex get_type_ast_type(AST_p ast)
{
   TypeIndex res = 0;

   switch(ast->type)
   {
   case t_STRING:
         res = T_String;
         break;
   case t_INTEGER:
         res = T_Integer;
         break;
   default:
         printf("Error: ast->type: %d\n", ast->type);
         assert(false && "Unexpected AST type in get_type_ast_type()");
         break;
   }
   return res;
}


/*-----------------------------------------------------------------------
//
// Function: st_insert_symbols()
//
//   Insert the variables defined in a single vardef (or param)
//   construct into the current symbol table.
//
// Global Variables:
//
// Side Effects    :
//
/----------------------------------------------------------------------*/

bool st_insert_symbols(SymbolTable_p st, TypeTable_p tt,
                       AST_p typenode, AST_p symbols)
{
   bool res;

   TypeIndex type = get_type_ast_type(typenode);

   //printf("st_insert_symbols:");ASTCellPrint(typenode);

   switch(symbols->type)
   {
   case idlist:
         res = st_insert_symbols(st, tt, typenode, symbols->child[0])
            && st_insert_symbols(st, tt, typenode, symbols->child[1]);
         break;
   case t_IDENT:
         res = STInsertSymbol(st, symbols->litval, type,
                              typenode->line, typenode->column);
         break;
   default:
         assert(false && "Unexpected AST type in st_insert_symbols()");
         break;

   }
   return res;
}


/*-----------------------------------------------------------------------
//
// Function: get_param_types()
//
//  Add the types of the parameters represented by ast to ntype.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

static void get_param_types(NanoType_p ntype, AST_p ast)
{
   if(ast)
   {
      switch(ast->type)
      {
      case params:
            get_param_types(ntype, ast->child[0]);
            break;
      case paramlist:
            get_param_types(ntype, ast->child[0]);
            get_param_types(ntype, ast->child[1]);
            break;
      case param:
            ntype->typeargs[ntype->typeargno++] =
               get_type_ast_type(ast->child[0]);
            break;
      default:
            assert(false && "Unexpected AST type in get_param_types()");
            break;
      }
   }

}


/*-----------------------------------------------------------------------
//
// Function: st_insert_params()
//
//   Insert the formal parameter list pointed to by ast into the
//   symbol table. Return success (true) or failure.
//
// Global Variables:
//
// Side Effects    :
//
/----------------------------------------------------------------------*/

static bool st_insert_params(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
   bool res = true;

   if(ast)
   {
      switch(ast->type)
      {
      case params:
            res = st_insert_params(st, tt, ast->child[0]) && res;
            break;
      case paramlist:
            res = st_insert_params(st, tt, ast->child[0]) && res;
            res = st_insert_params(st, tt, ast->child[1]) && res;
            break;
      case param:
            res = STInsertVarDef(st, tt, ast) && res;
            break;
      default:
            assert(false && "Unexpected AST type in get_param_types()");
            break;
      }
   }
   return res;
}


/*-----------------------------------------------------------------------
//
// Function: type_error()
//
//    Print an error describing the type discrepancy between expected
//    and node->result_type.
//
// Global Variables:
//
// Side Effects    :
//
/----------------------------------------------------------------------*/

static void type_error(FILE* out, TypeTable_p tt, TypeIndex expected, AST_p node)
{
   fprintf(out, "%d:%d: error: wrong type of expression - expected '",
           node->line, node->column);
   TypePrint(out, tt, expected);
   fprintf(out, "', found '");
   TypePrint(out, tt, node->result_type);
   fprintf(out, "'\n");
}


/*-----------------------------------------------------------------------
//
// Function: type_check_arglist()
//
//   Check if the types of the arguments in the actual argument list
//   starting at args corresponds to the type given in
//   fun_types. *position indicates the current position in the
//   argument list. If types disagree, emit an error message and
//   return false, otherwise return true.
//
// Global Variables:
//
// Side Effects    :
//
/----------------------------------------------------------------------*/

static bool type_check_arglist(FILE* out, char* function,
                               NanoType_p fun_type, TypeTable_p tt,
                               int* position, AST_p args)
{
   bool res = true;

   if(args)
   {
      /* ASTCellPrint(args);*/

      if(*position == fun_type->typeargno)
      {
         fprintf(out, "%d:%d: error: Too many arguments to call of function '%s'\n",
                 args->line, args->column, function);
         res = false;
      }
      else
      {
         switch(args->type)
         {
         case arglist:
               res = type_check_arglist(out, function, fun_type, tt,
                                        position, args->child[0]) && res;
               res = type_check_arglist(out, function, fun_type, tt,
                                        position, args->child[1]) && res;
               break;
         default:
               if(args->result_type != fun_type->typeargs[*position])
               {
                  type_error(out, tt, fun_type->typeargs[*position], args);
                  res = false;
               }
               (*position)++;
               break;
         }
      }
   }
   return res;
}



/*-----------------------------------------------------------------------
//
// Function: type_check_funcall()
//
//   Check correctness of a function call, i.e. check if the
//   identifier is defined, and then if the arguments come in the
//   right order. Return true if no error was found, false otherwise.
//
// Global Variables: -
//
// Side Effects    : Error messages.
//
/----------------------------------------------------------------------*/

static bool type_check_funcall(FILE* out, TypeTable_p tt, AST_p ast)
{
   bool res = true;
   Symbol_p entry;
   NanoType_p   fun_type;
   int argno = 1;

   assert(ast->context);

   entry = STFindSymbolGlobal(ast->context, ast->child[0]->litval);
   if(!entry)
   {
      fprintf(out, "%d:%d: error: undefined identifier %s\n",
              ast->child[0]->line, ast->child[0]->column, ast->child[0]->litval);
      return false;
   }
   fun_type = &(tt->types[entry->type]);
   ast->result_type = TypeRetType(fun_type);

   res =  type_check_arglist(out, ast->child[0]->litval,
                             fun_type, tt, &argno, ast->child[1]);
   if(argno < fun_type->typeargno)
   {
      res = false;
      fprintf(out, "%d:%d: error: too few arguments to call of function '%s'\n",
              ast->line, ast->column, ast->child[0]->litval);

   }
   return res;
}


/*-----------------------------------------------------------------------
//
// Function: body_has_return()
//
//   Return true if the body has an (unconditional) return statement.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

static bool body_has_return(AST_p ast)
{
   bool res = false;

   if(ast)
   {
      switch(ast->type)
      {
      case ret_stmt:
            res = true;
            break;
      case body:
            res = body_has_return(ast->child[1]);
            break;
      case stmts:
            res |= body_has_return(ast->child[0]);
            res |= body_has_return(ast->child[1]);
            break;
      default:
            break;
      }
   }
   return res;
}


bool STInsertVarDef(SymbolTable_p st, TypeTable_p tt, AST_p def)
{
   assert((def->type == vardef) || (def->type == param));

   //printf("# STInsertVarDef():");SExprASTPrint(stdout, def);printf("\n");
   //ASTCellPrint(def);
   //ASTCellPrint(def->child[0]);

   return  st_insert_symbols(st, tt, def->child[0], def->child[1]);
}

bool STInsertFunDef(SymbolTable_p st, TypeTable_p tt, AST_p def)
{
   NanoTypeCell ntype;
   TypeIndex    type;

   assert(def->type == fundef);

   // printf("# STInsertFunDef():");SExprASTPrint(stdout, def->child[0]);printf("\n");
   ntype.constructor = tc_function;
   ntype.typeargno   = 1;
   ntype.typeargs[0] = get_type_ast_type(def->child[0]);

   get_param_types(&ntype, def->child[2]);
   type = TypeTableGetTypeIndex(tt, &ntype);
   return STInsertSymbol(st, def->child[1]->litval, type,
                         def->child[0]->line, def->child[0]->column);
}


bool STBuildAllTables(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
   bool res = true;
   int  i;

   assert(st);

   if(ast)
   {
         //printf("\nSymboltable at node: ");
         //ASTCellPrint(ast);
         //SymbolTablePrintGlobal(stdout, st, tt);

      ast->context = st;
      switch(ast->type)
      {
      case vardef:
            res = STBuildAllTables(st, tt, ast->child[0]) && res;
            res = STBuildAllTables(st, tt, ast->child[1]) && res;
            res = STInsertVarDef(st, tt, ast) && res;
            break;
      case fundef:
            res = STInsertFunDef(st, tt, ast) && res;
            res = STBuildAllTables(st, tt, ast->child[0]) && res;
            res = STBuildAllTables(st, tt, ast->child[1]) && res;
            st = STEnterContext(st);
            res = st_insert_params(st, tt, ast->child[2]) && res;
            res = STBuildAllTables(st, tt, ast->child[2]) && res;
            res = STBuildAllTables(st, tt, ast->child[3]) && res;
            st = STLeaveContext(st);
            break;
      case body:
            st = STEnterContext(st);
            ast->context = st;
            res = STBuildAllTables(st, tt, ast->child[0]) && res;
            res = STBuildAllTables(st, tt, ast->child[1]) && res;
            st = STLeaveContext(st);
            break;
      default:
            for(i=0; ast->child[i]; i++)
            {
               res = STBuildAllTables(st, tt, ast->child[i]) && res;
            }
            break;
      }
   }
   return res;
}

TypeIndex GetSymbolResType(FILE* out, TypeTable_p tt, AST_p node)
{
   assert(node->type == t_IDENT);
   assert(node->context);

   Symbol_p entry = STFindSymbolGlobal(node->context, node->litval);
   if(!entry)
   {
      fprintf(out, "%d:%d: error: undefined identifier %s\n",
              node->line, node->column, node->litval);
      return T_NoType;
   }
   return TypeTableGetRetType(tt, entry->type);
}


bool ASTTypeCheck(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
   bool res = true;

   if(ast)
   {
      /* Types work bottom-up */
      res = ASTTypeCheck(st, tt, ast->child[0]) && res;
      res = ASTTypeCheck(st, tt, ast->child[1]) && res;
      res = ASTTypeCheck(st, tt, ast->child[2]) && res;
      res = ASTTypeCheck(st, tt, ast->child[3]) && res;
      switch(ast->type)
      {
      case t_INTLIT:
            ast->result_type = T_Integer;
            break;
      case t_STRINGLIT:
            ast->result_type = T_String;
            break;
      case t_IDENT:
            ast->result_type = GetSymbolResType(stdout, tt, ast);
            if(ast->result_type == T_NoType)
            {
               res = false;
            }
            break;
      case funcall:
            res = type_check_funcall(stdout, tt, ast);
            break;
      case t_MULT:
      case t_DIV:
      case t_PLUS:
      case t_MINUS:
            if(ast->child[0]->result_type != T_Integer)
            {
               type_error(stdout, tt, T_Integer, ast->child[0]);
               res = false;
            }
            if(ast->child[1] && (ast->child[1]->result_type != T_Integer))
            {
               type_error(stdout, tt, T_Integer, ast->child[1]);
               res = false;
            }
            ast->result_type = T_Integer;
            break;
      case t_EQ:
      case t_NEQ:
      case t_LT:
      case t_GT:
      case t_LEQ:
      case t_GEQ:
      case assign:
            /* Per syntax, we can only have expressions, which are
               T_String or T_Integer. */
            if(ast->child[0]->result_type != ast->child[1]->result_type)
            {
               /* We flag the second expression as wrong */
               type_error(stdout, tt, ast->child[0]->result_type, ast->child[1]);
               res = false;
            }
            ast->result_type = T_NoType;
            break;
      default:
            ast->result_type = T_NoType;
            break;
      }
   }
   return res;
}



/*-----------------------------------------------------------------------
//
// Function: ASTCheckReturnTypes()
//
//   Check if all returns have the proper type.
//
// Global Variables: -
//
// Side Effects    : Error output
//
/----------------------------------------------------------------------*/

bool ASTCheckReturnTypes(SymbolTable_p st, TypeTable_p tt,
                         TypeIndex expected, AST_p ast)
{
   bool res = true;

   if(ast)
   {
      switch(ast->type)
      {
      case fundef:
            assert(expected==T_NoType);
            expected = STSymbolReturnType(st, tt, ast->child[1]->litval);
            res = ASTCheckReturnTypes(st, tt, expected, ast->child[3]) && res;
            break;
      case ret_stmt:
            assert(expected!=T_NoType);
            if(expected != ast->child[0]->result_type)
            {
               type_error(stderr, tt, expected, ast->child[0]);
               res = false;
            }
            break;
      default:
            res = ASTCheckReturnTypes(st, tt, expected, ast->child[0]) && res;
            res = ASTCheckReturnTypes(st, tt, expected, ast->child[1]) && res;
            res = ASTCheckReturnTypes(st, tt, expected, ast->child[2]) && res;
            res = ASTCheckReturnTypes(st, tt, expected, ast->child[3]) && res;
            break;
      }
   }
   return res;
}


/*-----------------------------------------------------------------------
//
// Function: ASTCheckReturns()
//
//   Check if all functions have a return statement.
//
// Global Variables: -
//
// Side Effects    : Prints warning if statement is missing.
//
/----------------------------------------------------------------------*/

bool ASTCheckReturns(AST_p ast)
{
   bool res = true;

   if(ast)
   {
      switch(ast->type)
      {
      case prog:
            res = ASTCheckReturns(ast->child[0]) && res;
            res = ASTCheckReturns(ast->child[1]) && res;
            break;
      case fundef:
            if(!body_has_return(ast->child[3]))
            {
               fprintf(stdout, "%d:%d: warning: cannot guarantee proper "
                       "return value for function %s()\n",
                       ast->child[0]->line, ast->child[0]->column, ast->child[1]->litval);
               res = false;
            }
            break;
      default:
            break;
      }
   }
   return res;
}


/*-----------------------------------------------------------------------
//
// Function: STCheckMain()
//
//   Check if the symbol table contains a proper main() function with
//   a legal type (i.e. returns Integer, all arguments are String).
//
// Global Variables: -
//
// Side Effects    : Error output
//
/----------------------------------------------------------------------*/

bool STCheckMainTypes(SymbolTable_p st, TypeTable_p tt)
{
   Symbol_p   symbol;
   NanoType_p type;
   bool res = true;
   int i;

   symbol = STFindSymbolLocal(st, "main");
   if(!symbol)
   {
      fprintf(stderr, "error: no main function()\n");
      return false;
   }
   type = &(tt->types[symbol->type]);
   if(type->constructor!=tc_function)
   {
      fprintf(stderr, "%d:%d: error: main should be function\n",
              symbol->line, symbol->col);
      return false;
   }
   assert(type->typeargno);
   if(type->typeargs[0] !=T_Integer)
   {
      fprintf(stderr, "%d:%d: error: main() should return Integer\n",
              symbol->line, symbol->col);
      res = false;
   }
   for(i=1; i<type->typeargno; i++)
   {
      if(type->typeargs[i] != T_String)
      {
         fprintf(stderr, "%d:%d: error: All arguments to main() should be String\n",
                 symbol->line, symbol->col);
         res = false;
      }
   }

   return res;
}
