LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc
CFLAGS = -std=gnu99 -ggdb


.PHONY: all clean tags

all: exprcc

clean:
	@touch __dummy.o __dummy~ exprcc nanoparse.output TAGS nanoparse.tab.c
	rm nanoparse.tab.[ch] *.o *~ exprcc nanoparse.output TAGS

tags:
	etags *.c *.h *.y *.l

nanolex.c: nanolex.l nanoparse.tab.h

nanoparse.tab.h: nanoparse.y ast.h
	$(YACC) --verbose -d nanoparse.y

nanoparse.tab.c: nanoparse.y ast.h
	$(YACC) -d nanoparse.y

nanoparse.tab.o: nanoparse.tab.c

ast.o: ast.h ast.c

exprcc: nanolex.o nanoparse.tab.o ast.o
	$(LD) nanolex.o nanoparse.tab.o ast.o -o exprcc
