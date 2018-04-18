/*-----------------------------------------------------------------------

File  : symbols.c

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Implementation of symbol tables.

  Copyright 2015 by the author.
  This code is released under the GNU General Public Licence.

Changes

<1> Sun Apr 19 14:34:20 CEST 2015
    New

-----------------------------------------------------------------------*/

#include "symbols.h"



/*---------------------------------------------------------------------*/
/*                        Global Variables                             */
/*---------------------------------------------------------------------*/


/*---------------------------------------------------------------------*/
/*                      Forward Declarations                           */
/*---------------------------------------------------------------------*/


/*---------------------------------------------------------------------*/
/*                         Internal Functions                          */
/*---------------------------------------------------------------------*/



/*---------------------------------------------------------------------*/
/*                         Exported Functions                          */
/*---------------------------------------------------------------------*/

/*-----------------------------------------------------------------------
//
// Function: SymbolTableAlloc()
//
//   Return a empty and unattached symbol table.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

SymbolTable_p SymbolTableAlloc(void)
{
   SymbolTable_p handle = SymbolTableCellAlloc();

   handle->context    = NULL;
   handle->symbol_ctr = 0;

   return handle;
}


/*-----------------------------------------------------------------------
//
// Function: SymbolTableFree()
//
//   Free a symbol table (but not its contexts!)
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

void SymbolTableFree(SymbolTable_p junk)
{
   int i;

   if(junk)
   {
      for(i=0; i< junk->symbol_ctr; i++)
      {
         free(junk->symbols[i].symbol);
      }
   }
   SymbolTableCellFree(junk);
}


/*-----------------------------------------------------------------------
//
// Function: STEnterContext()
//
//   Enter a new context: Initialize a new ST, embed it in the
//   existing context, return a pointer to it.
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/

SymbolTable_p STEnterContext(SymbolTable_p table)
{
   SymbolTable_p handle = SymbolTableAlloc();

   handle->context = table;

   return handle;
}

/*-----------------------------------------------------------------------
//
// Function: STLeaveContext()
//
//   Leave a context. Does not free the old symbol table, nor changes
//   its context, as these will be added as annotations to the AST.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

SymbolTable_p STLeaveContext(SymbolTable_p table)
{
   return table->context;
}


/*-----------------------------------------------------------------------
//
// Function: STFindSymbolLocal()
//
//   Find a symbol locally and return it. Return NULL if it does not
//   exist.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

Symbol_p STFindSymbolLocal(SymbolTable_p table, char* symbol)
{
   int i;

   for(i=0; i< table->symbol_ctr; i++)
   {
      if(strcmp(table->symbols[i].symbol, symbol) == 0)
      {
         return &(table->symbols[i]);
      }
   }
   return NULL;
}


/*-----------------------------------------------------------------------
//
// Function: STFindSymbolGlobal()
//
//   Find a symbol in the current hierarchy of contexts. Return NULL
//   if it does not exist.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

Symbol_p STFindSymbolGlobal(SymbolTable_p table, char* symbol)
{
   Symbol_p res;

   if(!table)
   {
      return NULL;
   }
   res = STFindSymbolLocal(table, symbol);

   if(!res)
   {
      res = STFindSymbolGlobal(table->context, symbol);
   }
   return res;
}

/*-----------------------------------------------------------------------
//
// Function: STSymbolReturnType()
//
//   Return the return type of the identifier in the current scope, or
//   T_NoType if the symbol is unknown.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

TypeIndex STSymbolReturnType(SymbolTable_p table, TypeTable_p tt, char* symbol)
{
   Symbol_p entry = STFindSymbolGlobal(table, symbol);

   if(!entry)
   {
      return T_NoType;
   }
   return TypeTableGetRetType(tt, entry->type);
}


/*-----------------------------------------------------------------------
//
// Function: STInsertSymbol()
//
//   Insert a symbol into the symbol table. Return true on success,
//   print error and return false otherwise.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

bool STInsertSymbol(SymbolTable_p table, char* symbol, TypeIndex type,
                    int line, int col)
{
   Symbol_p handle;

   handle = STFindSymbolLocal(table, symbol);
   if(handle)
   {
      printf("%d:%d: error: symbol '%s' doubly defined (previous "
             "definition at %d:%d)\n",
             line, col, symbol, handle->line, handle->col);
      // exit(EXIT_FAILURE);
      return false;
   }
   if(table->symbol_ctr==MAXSYMBOLS)
   {
      fprintf(stderr, "Symbol table overflow: Too many symbols in one context!\n");
      exit(EXIT_FAILURE);
   }
   table->symbols[table->symbol_ctr].symbol = strdup(symbol);
   table->symbols[table->symbol_ctr].line   = line;
   table->symbols[table->symbol_ctr].col    = col;
   table->symbols[table->symbol_ctr].type   = type;

   table->symbol_ctr++;

   return true;
}


/*-----------------------------------------------------------------------
//
// Function: SymbolTablePrintLocal()
//
//   Print a symbol table (without context).
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void SymbolTablePrintLocal(FILE* out, SymbolTable_p st, TypeTable_p tt)
{
   int i;

   for(i=0; i< st->symbol_ctr; i++)
   {
      fprintf(out, "%-20s: ", st->symbols[i].symbol);
      TypePrint(out, tt, st->symbols[i].type);
      fprintf(out, "\n");
   }
}


/*-----------------------------------------------------------------------
//
// Function: SymbolTablePrintGlobal()
//
//   Print a symbol table (with full context. i.e. all symbol tables
//   of enclosing contexts).
//
// Global Variables:
//
// Side Effects    :
//
/----------------------------------------------------------------------*/

void SymbolTablePrintGlobal(FILE* out, SymbolTable_p st, TypeTable_p tt)
{
   if(st)
   {
      fprintf(out, "-----------------------\n");
      SymbolTablePrintLocal(out, st, tt);
      SymbolTablePrintGlobal(out, st->context, tt);
   }
}



/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
