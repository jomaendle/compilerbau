LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc

all: nanoLangCompiler


myflex.c: tokenizer.l parser.tab.h

parser.tab.h: parser.y
	$(YACC) -d parser.y

parser.tab.c: parser.y
	$(YACC) -d parser.y

parser.tab.o: parser.tab.c

nanoLangCompiler: myflex.o parser.tab.o
	$(LD) myflex.o parser.tab.o -o nanoLangCompiler
