LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc
CFLAGS = -std=gnu99 -ggdb


.PHONY: all clean tags

all: ncc

clean:
	@touch __dummy.o __dummy~ ncc nanoparse.output TAGS nanoparse.tab.c
	rm nanoparse.tab.[ch] *.o *~ ncc nanoparse.output TAGS

tags:
	etags *.c *.h *.y *.l

nanolex.c: nanolex.l nanoparse.tab.h

nanoparse.tab.h: nanoparse.y ast.h types.h semantic.h symbols.h
	$(YACC) --verbose -d nanoparse.y

nanoparse.tab.c: nanoparse.y ast.h
	$(YACC) -d nanoparse.y

nanoparse.tab.o: nanoparse.tab.c

ast.o: ast.h ast.c

types.o: types.c types.h

symbols.o: symbols.c symbols.h types.h

semantic.o: ast.h types.h symbols.h semantic.h

ncc: nanolex.o nanoparse.tab.o ast.o types.o symbols.o semantic.o
	$(LD) nanolex.o nanoparse.tab.o ast.o types.o symbols.o semantic.o -o ncc
