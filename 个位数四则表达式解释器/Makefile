parser: parser.tab.c lex.yy.c expression.c
	gcc parser.tab.c lex.yy.c expression.c -o parser

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lex.l
	flex lex.l

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c parser
