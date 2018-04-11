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

nanoLangParser.tab.h: nanoLangParser.y ast.h
	$(YACC) --verbose -d nanoLangParser.y

nanoLangParser.tab.c: nanoLangParser.y ast.h
	$(YACC) -d nanoLangParser.y

nanoLangParser.tab.o: nanoLangParser.tab.c

ast.o: ast.h ast.c

nanoLangCompiler: nanoLangScanner.o nanoLangParser.tab.o ast.o
	$(LD) nanoLangScanner.o nanoLangParser.tab.o ast.o -o nanoLangCompiler
