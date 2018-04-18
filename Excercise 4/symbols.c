
#include "symbols.h"

SymbolTable_p SymbolTableAlloc(void)
{
   SymbolTable_p handle = SymbolTableCellAlloc();

   handle->context    = NULL;
   handle->symbol_ctr = 0;

   return handle;
}

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

SymbolTable_p STEnterContext(SymbolTable_p table)
{
   SymbolTable_p handle = SymbolTableAlloc();

   handle->context = table;

   return handle;
}

SymbolTable_p STLeaveContext(SymbolTable_p table)
{
   return table->context;
}

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

TypeIndex STSymbolReturnType(SymbolTable_p table, TypeTable_p tt, char* symbol)
{
   Symbol_p entry = STFindSymbolGlobal(table, symbol);

   if(!entry)
   {
      return T_NoType;
   }
   return TypeTableGetRetType(tt, entry->type);
}

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

void SymbolTablePrintLocal(FILE* out, SymbolTable_p st, TypeTable_p tt)
{
   int i;

   for(i=0; i< st->symbol_ctr; i++)
   {
      fprintf(out, "%-20s: ", st->symbols[i].symbol);
      TypePrint(out, tt, st->symbols[i].type);
      //TypePrintNumber(out, tt, st->symbols[i].type);
      fprintf(out, "\n");
   }
}

void SymbolTablePrintGlobal(FILE* out, SymbolTable_p st, TypeTable_p tt)
{
   if(st)
   {
      fprintf(out, "-----------------------\n");
      SymbolTablePrintLocal(out, st, tt);
      SymbolTablePrintGlobal(out, st->context, tt);

   }
}
