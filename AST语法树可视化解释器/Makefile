# ---------- Makefile ----------

parser: parser.tab.c lex.yy.c displayAST.c
	gcc parser.tab.c lex.yy.c displayAST.c -o parser

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lex.l
	flex lex.l

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c parser

# ---------- End of Makefile ----------