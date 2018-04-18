
#include "types.h"
#include "ast.h"

char* atomic_type_names[] =
{
   "NoType",
   "String",
   "Integer"
};

int TypeCmp(NanoType_p t1, NanoType_p t2){
  int i =0 ;
  if (t1 && t2 != NULL){
    if(t1->constructor == t2->constructor){
      if(t1->typeargno == t2->typeargno){
        for(i=0; i<t1->typeargno; i++){
          if(t1->typeargs[i] != t2->typeargs[i]){
            return 0;
          }
          return 1;
        }
      }
    }
  }
  return 0;
}

void BuildTypeTable(AST_p ast, TypeTable_p table){
  if(ast->type == prog){
    BuildTypeTable(ast, table);
    if(ast->child[1]->type == fundef){
      NanoTypeCell cell;
      cell.constructor = tc_function;
      cell.typeargs[0] = ast->child[1]->child[0]->type;

      int i =0;
      cell.typeargno = 1;
      for (i=1; i<5; i++){
        if(ast->child[1]->child[2]->child[i-1]){
          cell.typeargs[i] = ast->child[1]->child[2]->child[i-1]->type;
          cell.typeargno++;
        }
      }
      TypeTableInsert(table, &cell);
    }
  }

}


TypeIndex TypeTableGetRetType(TypeTable_p table, TypeIndex type)
{
   assert((type>0) && (type<table->type_ctr));

   return TypeRetType(&(table->types[type]));
}

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

void TypeTableInit(TypeTable_p table){
    NanoTypeCell type;
    table->type_ctr = 0;

    type.constructor = tc_atomic;
    type.typeargno   = 1;
    type.typeargs[0] = T_NoType;
    TypeTableInsert(table, &type);

    type.typeargs[0] = T_String;
    TypeTableInsert(table, &type);

    type.typeargs[0] = T_Integer;
    TypeTableInsert(table, &type);

}

TypeTable_p  TypeTableAlloc(){

  TypeTable_p type;

  type = TypeTableCellAlloc();
  TypeTableInit(type);

  return type;
}


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

TypeIndex  TypeTableInsert(TypeTable_p table, NanoType_p type){
  if (table->type_ctr == MAXTYPES) {
    printf("Count MaxTypes reached");
    return -1;
  }
  table->types[table->type_ctr];
  table->type_ctr++;
  return table->type_ctr-1;
}


TypeIndex  TypeTableGetTypeIndex(TypeTable_p table, NanoType_p type){
  int i = 0;
  for(i =0; i<table->type_ctr; i++){
    if(TypeCmp(type, &(table->types[i]) == 1)){
      return table->type_ctr;
    }else{
      printf("Type not found.");
      return -1;
    }
  }
}
