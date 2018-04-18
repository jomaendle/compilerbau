/*-----------------------------------------------------------------------

File  : types.c

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Type system for nanoLang.

  Copyright 2015-2017 by the author.
  This code is released under the GNU General Public Licence.

Changes

<1> Wed Apr 15 16:41:47 CEST 2015
    New

-----------------------------------------------------------------------*/

#include "types.h"



/*---------------------------------------------------------------------*/
/*                        Global Variables                             */
/*---------------------------------------------------------------------*/

char* atomic_type_names[] =
{
   "NoType",
   "String",
   "Integer"
};

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
// Function: TypeCmp()
//
//   Compare to types. Return 0 if they are equal, !=0 otherwise.
//
// Global Variables:
//
// Side Effects    :
//
/----------------------------------------------------------------------*/

int TypeCmp(NanoType_p t1, NanoType_p t2)
{
   int res, i;

   res = t1->constructor - t2->constructor;
   if(res)
   {
      return res;
   }
   res = t1->typeargno - t2->typeargno;
   if(res)
   {
      return res;
   }
   for(i=0; i<t1->typeargno; i++)
   {
      res = t1->typeargs[i] - t2->typeargs[i];
      if(res)
      {
         break;
      }
   }
   return res;
}


/*-----------------------------------------------------------------------
//
// Function: TypePrint()
//
//   Print a type in readable form (for debugging).
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void TypePrint(FILE* out, TypeTable_p table, TypeIndex type)
{
   int i;
   char* sep = "";

   switch(table->types[type].constructor)
   {
   case tc_atomic:
         fprintf(out, "%s", atomic_type_names[type]);
         break;
   case tc_function:
         fprintf(out, "(");
         for(i=1; i < table->types[type].typeargno; i++)
         {
            fprintf(out, "%s", sep);
            TypePrint(out, table, table->types[type].typeargs[i]);
            sep = ", ";
         }
         fprintf(out, ") -> ");
         TypePrint(out, table, table->types[type].typeargs[0]);
         break;
   default:
         printf("Error, unknown type constructor!\n");
         break;
   }
}

/*-----------------------------------------------------------------------
//
// Function: TypeTableInit()
//
//   Initialize a Type Table by inserting the first two types (String
//   and Integer) into the table.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

void TypeTableInit(TypeTable_p table)
{
   NanoTypeCell type;

   table->type_ctr=0;

   /* Insert dummy no-type */
   type.constructor = tc_atomic;
   type.typeargno   = 1;
   type.typeargs[0] = T_NoType;
   TypeTableInsert(table, &type);

   /* Insert atomoic types - note that constructor and argno are
    * still set from above. */
   type.typeargs[0] = T_String;
   TypeTableInsert(table, &type);

   type.typeargs[0] = T_Integer;
   TypeTableInsert(table, &type);
}


/*-----------------------------------------------------------------------
//
// Function: TypeTableAlloc()
//
//   Allocate and initialise a type table.
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/

TypeTable_p  TypeTableAlloc()
{
   TypeTable_p handle;

   handle = TypeTableCellAlloc();
   TypeTableInit(handle);

   return handle;
}


/*-----------------------------------------------------------------------
//
// Function: TypeTableFree()
//
//   Free a type table.
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/

void TypeTableFree(TypeTable_p junk)
{
   TypeTableCellFree(junk);
}

/*-----------------------------------------------------------------------
//
// Function: TypeTablePrint()
//
//   Print a type table.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void TypeTablePrint(FILE* out, TypeTable_p table)
{
   int i;

   for(i=0; i<table->type_ctr; i++)
   {
      fprintf(out, "%d: ", i);
      TypePrint(out, table, i);
      fprintf(out, "\n");
   }
}


/*-----------------------------------------------------------------------
//
// Function: TypeTableInsert()
//
//   Insert a type into the table and return its index. Does not check
//   for duplicates. See below (TypeTableGetTypeIndex()) for the more
//   usual interface.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/


TypeIndex TypeTableInsert(TypeTable_p table, NanoType_p type)
{
   if(table->type_ctr==MAXTYPES)
   {
      fprintf(stderr, "Type table overflow: Out of types!\n");
      exit(EXIT_FAILURE);
   }
   table->types[table->type_ctr] = *type;
   table->type_ctr++;

   return table->type_ctr-1;
}

/*-----------------------------------------------------------------------
//
// Function: TypeTableGetTypeIndex()
//
//   Given a type, find or insert it into the type table, and return
//   the index of the type.
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

TypeIndex TypeTableGetTypeIndex(TypeTable_p table, NanoType_p type)
{
   int i;
   for(i=0; i<table->type_ctr; i++)
   {
      if(TypeCmp(type, &(table->types[i])) == 0)
      {
         return i;
      }
   }
   return TypeTableInsert(table, type);
}


/*-----------------------------------------------------------------------
//
// Function: TypeTableGetRetType()
//
//   Given a type index, return the return type of an object of this
//   type (for functions, this is return type of the function, for
//   variables the type of the variable).
//
// Global Variables: -
//
// Side Effects    : -
//
/----------------------------------------------------------------------*/

TypeIndex TypeTableGetRetType(TypeTable_p table, TypeIndex type)
{
   assert((type>0) && (type<table->type_ctr));

   return TypeRetType(&(table->types[type]));
}


/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
