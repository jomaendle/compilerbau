LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc
CFLAGS = -std=gnu99 -ggdb


.PHONY: all clean tags

all: nanoLangCompiler

clean:
	@touch __dummy.o __dummy~ nanoLangCompiler nanoLangParser.output TAGS nanoLangParser.tab.c
	rm nanoLangParser.tab.[ch] *.o *~ nanoLangCompiler nanoLangParser.output TAGS

tags:
	etags *.c *.h *.y *.l

nanoLangScanner.c: nanoLangScanner.l nanoLangParser.tab.h

nanoLangParser.tab.h: nanoLangParser.y ast.h types.h semantic.h symbols.h
	$(YACC) --verbose -d nanoLangParser.y

nanoLangParser.tab.c: nanoLangParser.y ast.h
	$(YACC) -d nanoLangParser.y

nanoLangParser.tab.o: nanoLangParser.tab.c

ast.o: ast.h ast.c

types.o: types.c types.h

symbols.o: symbols.c symbols.h types.h

semantic.o: ast.h types.h symbols.h semantic.h

nanoLangCompiler: nanoLangScanner.o nanoLangParser.tab.o ast.o types.o symbols.o semantic.o
	$(LD) nanoLangScanner.o nanoLangParser.tab.o ast.o types.o symbols.o semantic.o -o nanoLangCompiler
