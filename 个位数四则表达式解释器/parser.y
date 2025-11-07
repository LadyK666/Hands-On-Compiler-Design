%{
#include <stdio.h>
#include <stdlib.h>
#include "def.h"

extern int yylex(void);
extern int yyparse(void);
extern FILE *yyin;
extern char *yytext;
void yyerror(const char *s);
%}

%code requires {
#include "def.h"
}

%union {
    ASTNode *node;
    int value;
}

%token DIGIT PLUS MINUS TIMES DIVIDE LPAREN RPAREN

%type <node> expr term factor

%%

input:
    expr {
        printf("\n=== 语法分析树 ===\n");
        print_tree($1, 0);
        printf("\n=== 表达式结果 ===\n");
        int result = evaluate($1);
        printf("结果: %d\n", result);
        free_tree($1);
    }
    ;

expr:
    expr PLUS term {
        $$ = create_op_node(NODE_PLUS, $1, $3);
    }
    | expr MINUS term {
        $$ = create_op_node(NODE_MINUS, $1, $3);
    }
    | term {
        $$ = $1;
    }
    ;

term:
    term TIMES factor {
        $$ = create_op_node(NODE_TIMES, $1, $3);
    }
    | term DIVIDE factor {
        $$ = create_op_node(NODE_DIVIDE, $1, $3);
    }
    | factor {
        $$ = $1;
    }
    ;

factor:
    DIGIT {
        $$ = create_digit_node(atoi(yytext));
    }
    | LPAREN expr RPAREN {
        $$ = $2;
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "语法错误: %s\n", s);
}

int main(int argc, char **argv) {
    printf("请输入一个四则表达式，例如：3+5*(2-1)\n");
    printf("=== 词法分析 ===\n");
    yyparse();
    return 0;
}

