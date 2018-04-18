/*-----------------------------------------------------------------------

File  : semantic.h

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Semantic analysis of nanoLang

  Copyright 2015 by the author.
  This code is released under the GNU General Public Licence.
  See the file COPYING in the main CLIB directory for details.
  Run "eprover -h" for contact information.

Changes

<1> Sun Apr 19 19:07:57 CEST 2015
    New

-----------------------------------------------------------------------*/

#ifndef SEMANTIC

#define SEMANTIC

#include "ast.h"
#include "symbols.h"


/*---------------------------------------------------------------------*/
/*                    Data type declarations                           */
/*---------------------------------------------------------------------*/



/*---------------------------------------------------------------------*/
/*                Exported Functions and Variables                     */
/*---------------------------------------------------------------------*/

bool STInsertVarDef(SymbolTable_p st, TypeTable_p tt, AST_p def);
bool STInsertFunDef(SymbolTable_p st, TypeTable_p tt, AST_p def);
bool STBuildAllTables(SymbolTable_p st, TypeTable_p tt, AST_p ast);

TypeIndex GetSymbolResType(FILE* out, TypeTable_p tt, AST_p node);
bool ASTTypeCheck(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool ASTCheckReturnTypes(SymbolTable_p st, TypeTable_p tt,
                         TypeIndex expected, AST_p ast);
bool ASTCheckReturns(AST_p ast);

bool STCheckMainTypes(SymbolTable_p st, TypeTable_p tt);


#endif

/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
