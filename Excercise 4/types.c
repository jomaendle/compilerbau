#include "types.h"
#include "symbols.h"

char* atomic_type_names[] = {
   "NoType",
   "String",
   "Integer"
};


int TypeCmp(NanoType_p t1, NanoType_p t2) {
    if(t1->constructor != t2->constructor || t1->typeargno != t2->typeargno) {
      return -1;
    }
   int i;
   for(i = 0; i < t1->typeargno; i++) {
      if(t1->typeargs[i] != t2->typeargs[i]) {
         return -1;
      }
   }
   return 0;
}

void TypePrint(FILE* out, TypeTable_p table, TypeIndex type) {
   switch(table->types[type].constructor) {
   case tc_atomic:
         fprintf(out, "%s", atomic_type_names[type]);
         break;
   case tc_function:
         fprintf(out, "(");
         int i;
         for(i = table->types[type].typeargno - 1; i >= 2; i--) {
            TypePrint(out, table, table->types[type].typeargs[i]);
            fprintf(out, "%s", ", ");
         }
         if(table->types[type].typeargno > 1) {
           TypePrint(out, table, table->types[type].typeargs[1]);
         }
         fprintf(out, ") -> ");
         TypePrint(out, table, table->types[type].typeargs[0]);
         break;
   default:
         printf("Error in type constructor!\n");
         break;
   }
}


void TypePrintNumber(FILE* out, TypeTable_p table, TypeIndex type) {
   switch(table->types[type].constructor) {
   case tc_atomic:
         fprintf(out, "%d", type);
         break;
   case tc_function:
         fprintf(out, "(");
         int i;
         for(i = table->types[type].typeargno - 1; i >= 2; i--) {
            TypePrintNumber(out, table, table->types[type].typeargs[i]);
            fprintf(out, "%s", ", ");
         }
         if(table->types[type].typeargno > 1) {
           TypePrintNumber(out, table, table->types[type].typeargs[1]);
         }
         fprintf(out, ") -> ");
         TypePrintNumber(out, table, table->types[type].typeargs[0]);
         break;
   default:
         printf("Error in type constructor!\n");
         break;
   }
}

void TypeTableInit(TypeTable_p table) {
   NanoTypeCell type;

   table->type_ctr = 0;

   // Insert NoType, Integer and String
   type.constructor = tc_atomic;
   type.typeargno   = 1;
   type.typeargs[0] = T_NoType;
   TypeTableInsert(table, &type);
   type.typeargs[0] = T_String;
   TypeTableInsert(table, &type);
   type.typeargs[0] = T_Integer;
   TypeTableInsert(table, &type);
}

TypeTable_p  TypeTableAlloc() {
   TypeTable_p table;
   table = TypeTableCellAlloc();
   TypeTableInit(table);
   return table;
}

void TypeTableFree(TypeTable_p junk) {
   TypeTableCellFree(junk);
}

void TypeTablePrint(FILE* out, TypeTable_p table) {
   int i;
   for(i = 0; i < table->type_ctr; i++) {
      fprintf(out, "%d: ", i);
      TypePrint(out, table, i);
      fprintf(out, "\n");
   }
}

TypeIndex TypeTableInsert(TypeTable_p table, NanoType_p type) {
   if(table->type_ctr == MAXTYPES) {
      fprintf(stderr, "No more free entry in the type table!\n");
      exit(EXIT_FAILURE);
   }
   table->types[table->type_ctr] = *type;
   table->type_ctr++;

   return table->type_ctr-1;
}

TypeIndex TypeTableGetTypeIndex(TypeTable_p table, NanoType_p type) {
   int i;
   for(i = 0; i < table->type_ctr; i++) {
      if(TypeCmp(type, &(table->types[i])) == 0) {
         return i;
      }
   }
   return TypeTableInsert(table, type);
}

TypeIndex TypeTableGetRetType(TypeTable_p table, TypeIndex type) {
   assert((type>0) && (type<table->type_ctr));
   return TypeRetType(&(table->types[type]));
}

/*
void BuildTypeTable(AST_p ast, TypeTable_p table) {
  if(ast->type==nil) return;
  if(ast->type == prog) {
    BuildTypeTable(ast->child[0], table);
    if(ast->child[1]->type == fundef) {
      NanoTypeCell cell;
      cell.constructor = tc_function;
      cell.typeargno = 1;
      if(ast->child[1]->child[0]->type == t_INTEGER) {
        cell.typeargs[0] = T_Integer;
      }
      if(ast->child[1]->child[0]->type == t_STRING) {
        cell.typeargs[0] = T_String;
      }
      if(ast->child[1]->child[2]->child[0]) {
        AST_p tempAst = ast->child[1]->child[2]->child[0];
        int i = 1;
        while(tempAst->type == paramlist) {
          if(tempAst->child[1]->child[0]->type == t_INTEGER) {
            cell.typeargs[i] = T_Integer;
            cell.typeargno++;
          }
          if(tempAst->child[1]->child[0]->type == t_STRING) {
            cell.typeargs[i] = T_String;
            cell.typeargno++;
          }
          i++;
          tempAst = tempAst->child[0];
        }
        if(tempAst->child[0]->type == t_INTEGER) {
          cell.typeargs[i] = T_Integer;
          cell.typeargno++;
        }
        if(tempAst->child[0]->type == t_STRING) {
          cell.typeargs[i] = T_String;
          cell.typeargno++;
        }
      }
    TypeTableGetTypeIndex(table, &cell);
    }
  }
}*/
