/*-----------------------------------------------------------------------

File  : symbols.h

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Simple data types for handling nanoLang symbol tables.

  Copyright 2015-2017 by the author.
  This code is released under the GNU General Public Licence.
  See the file COPYING in the main CLIB directory for details.
  Run "eprover -h" for contact information.

Changes

<1> Sun Apr 19 14:26:01 CEST 2015
    New

-----------------------------------------------------------------------*/

#ifndef SYMBOLS

#define SYMBOLS

#include <string.h>
#include <stdbool.h>
#include "types.h"


/*---------------------------------------------------------------------*/
/*                    Data type declarations                           */
/*---------------------------------------------------------------------*/

/* How many symbols can be in a single symbol table (i.e. context
 * without subcontexts) ? For a production  level program, this should
 * be dynamic */
#define MAXSYMBOLS 512

/* Storing information for one symbol */

typedef struct symbol
{
   char      *symbol;
   int       line; /* Where is this defined? */
   int       col;
   TypeIndex type;
}SymbolCell, *Symbol_p;

/* A symbol table table */

typedef struct symboltable
{
   struct symboltable *context;
   int                symbol_ctr;
   SymbolCell         symbols[MAXSYMBOLS];
}SymbolTableCell, *SymbolTable_p;



/*---------------------------------------------------------------------*/
/*                Exported Functions and Variables                     */
/*---------------------------------------------------------------------*/

#define SymbolTableCellAlloc()  (SymbolTableCell*)malloc(sizeof(SymbolTableCell))
#define SymbolTableCellFree(junk) free(junk)

SymbolTable_p SymbolTableAlloc(void);
void          SymbolTableFree(SymbolTable_p junk);

SymbolTable_p STEnterContext(SymbolTable_p table);
SymbolTable_p STLeaveContext(SymbolTable_p table);

Symbol_p  STFindSymbolLocal(SymbolTable_p table, char* symbol);
Symbol_p  STFindSymbolGlobal(SymbolTable_p table, char* symbol);
TypeIndex STSymbolReturnType(SymbolTable_p table, TypeTable_p tt, char* symbol);
bool      STInsertSymbol(SymbolTable_p table, char* symbol, TypeIndex type,
                         int line, int col);
void      SymbolTablePrintLocal(FILE* out, SymbolTable_p st, TypeTable_p tt);
void      SymbolTablePrintGlobal(FILE* out, SymbolTable_p st, TypeTable_p tt);


#endif


/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
