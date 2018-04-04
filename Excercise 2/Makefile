LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc

all: nanoLangCompiler

clean:
		@touch __dummy.o __dummy~ nanoLangCompiler nanoparse.output TAGS parser.tab.c
		rm parser.tab.[ch] *.o *~ nanoLangCompiler nanoparse.output TAGS

tags:
		etags *.c *.h *.y *.l


myflex.c: tokenizer.l parser.tab.h

parser.tab.h: parser.y ast.h
	$(YACC) --verbose -d parser.y

parser.tab.c: parser.y ast.h
	$(YACC) -d parser.y

ast.o: ast.h ast.c

parser.tab.o: parser.tab.c

nanoLangCompiler: myflex.o parser.tab.o ast.o
	$(LD) myflex.o parser.tab.o ast.o -o nanoLangCompiler
