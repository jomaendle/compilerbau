LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc
CFLAGS = -std=gnu99 -ggdb

.PHONY: all clean tags

all: nanoLangCompiler

clean:
		@touch __dummy.o __dummy~ nanoLangCompiler parser.output TAGS parser.tab.c
		rm parser.tab.[ch] *.o *~ nanoLangCompiler parser.output TAGS

tags:
		etags *.c *.h *.y *.l


myflex.c: tokenizer.l parser.tab.h

parser.tab.h: parser.y ast.h
	$(YACC) --verbose -d parser.y

parser.tab.c: parser.y ast.h
	$(YACC) -d parser.y

parser.tab.o: parser.tab.c

ast.o: ast.h ast.c

nanoLangCompiler: myflex.o parser.tab.o ast.o
	$(LD) myflex.o parser.tab.o ast.o -o nanoLangCompiler
