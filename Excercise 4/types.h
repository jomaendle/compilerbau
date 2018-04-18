/*-----------------------------------------------------------------------

File  : types.h

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Simple data types for handling nanoLang types.

  Copyright 2015-2017 by the author.
  This code is released under the GNU General Public Licence.
  See the file COPYING in the main CLIB directory for details.
  Run "eprover -h" for contact information.

Changes

<1> Wed Apr 15 16:15:50 CEST 2015
    New

-----------------------------------------------------------------------*/

#ifndef TYPES

#define TYPES

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/*---------------------------------------------------------------------*/
/*                    Data type declarations                           */
/*---------------------------------------------------------------------*/

/* How many arguments can a type constructor have? */
#define MAXTYPEARGS 256

/* Maximal number of types allowed in a nanoLang program */
#define MAXTYPES   1024

/* How can I construct new types? */
typedef enum
{
   tc_atomic,
   tc_function
}TypeConst;


/* Types are encoded as integers */
typedef enum
{
   T_NoType,
   T_String,
   T_Integer
} TypeIndex;


/* Storing information for one type. For convenience, we alwasys
 * encode the return type as the first entry in the typeargs, i.e. a
 * function type String, String -> Integer is encoded with
   constructor = tc_function
   typeargno   =  3 (2 arguments, return type)
   typeargs[]  = 2, 1, 1 (T_Integer, T_String, T_String)
*/

typedef struct nanotype
{
   TypeConst constructor;
   int       typeargno;
   TypeIndex typeargs[MAXTYPEARGS];
}NanoTypeCell, *NanoType_p;

/* A complete type table */

typedef struct typetable
{
   int          type_ctr;
   NanoTypeCell types[MAXTYPES];
}TypeTableCell, *TypeTable_p;



/*---------------------------------------------------------------------*/
/*                Exported Functions and Variables                     */
/*---------------------------------------------------------------------*/

#define TypeTableCellAlloc()  (TypeTableCell*)malloc(sizeof(TypeTableCell))
#define TypeTableCellFree(junk) free(junk)


int          TypeCmp(NanoType_p t1, NanoType_p t2);
#define      TypeRetType(t) ((t)->typeargs[0])
void         TypePrint(FILE* out, TypeTable_p table, TypeIndex type);

void         TypeTableInit(TypeTable_p table);
TypeTable_p  TypeTableAlloc(void);
void         TypeTableFree(TypeTable_p junk);
void         TypeTablePrint(FILE* out, TypeTable_p table);


TypeIndex    TypeTableInsert(TypeTable_p table, NanoType_p type);
TypeIndex    TypeTableGetTypeIndex(TypeTable_p table, NanoType_p type);
TypeIndex    TypeTableGetRetType(TypeTable_p table, TypeIndex type);

//void BuildTypeTable(AST_p ast, TypeTable_p table);

#endif

/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
