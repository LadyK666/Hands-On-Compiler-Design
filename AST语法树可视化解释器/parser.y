%define parse.error verbose
%locations

%code requires {
#include "def.h"
}

%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "def.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char* fmt, ...);
int yylex();
%}

/*********************请在下面完成parser.y*******************/

%union {
    ASTNode* type_node;
    int type_int;
    float type_float;
    char type_id[32];
}

%token LC RC LP RP SEMI
%token <type_int> INT
%token <type_float> FLOAT
%token <type_id> ID
%token IF ELSE WHILE SCAN PRINT
%token ASSIGNOP PLUS MINUS STAR DIV
%token EQ NE GT GE LT LE

%type <type_node> program StmList Stmt CompSt Exp

%expect 1

%left ASSIGNOP
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left STAR DIV
%right UMINUS

%%

program:
    StmList {
        $$ = createNode(NODE_PROGRAM);
        $$->child = $1;
        displayAST($$, 0);
    }
    ;

StmList:
    /* 空 */
    {
        $$ = NULL;
    }
    | Stmt StmList
    {
        $$ = createNode(NODE_STMLIST);
        $$->child = $1;
        $$->right = $2;
    }
    ;

CompSt:
    LC StmList RC
    {
        $$ = createNode(NODE_COMP_STMT);
        $$->child = $2;
    }
    ;

Stmt:
    Exp SEMI
    {
        $$ = createNode(NODE_EXP_STMT);
        $$->child = $1;
    }
    | CompSt
    {
        $$ = $1;
    }
    | SCAN ID SEMI
    {
        $$ = createNode(NODE_SCAN_STMT);
        strcpy($$->value.var_name, $2);
    }
    | PRINT Exp SEMI
    {
        $$ = createNode(NODE_PRINT_STMT);
        $$->child = $2;
    }
    | IF LP Exp RP Stmt
    {
        $$ = createNode(NODE_IF_STMT);
        $$->child = $3;
        $$->left = $5;
    }
    | IF LP Exp RP Stmt ELSE Stmt
    {
        $$ = createNode(NODE_IF_ELSE_STMT);
        $$->child = $3;
        $$->left = $5;
        $$->right = $7;
    }
    | WHILE LP Exp RP Stmt
    {
        $$ = createNode(NODE_WHILE_STMT);
        $$->child = $3;
        $$->left = $5;
    }
    ;

Exp:
    INT
    {
        $$ = createIntNode($1);
    }
    | FLOAT
    {
        $$ = createFloatNode($1);
    }
    | ID
    {
        $$ = createVarNode($1);
    }
    | Exp ASSIGNOP Exp
    {
        $$ = createNode(NODE_ASSIGN);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp PLUS Exp
    {
        $$ = createNode(NODE_PLUS);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp MINUS Exp
    {
        $$ = createNode(NODE_MINUS);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp STAR Exp
    {
        $$ = createNode(NODE_STAR);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp DIV Exp
    {
        $$ = createNode(NODE_DIV);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp EQ Exp
    {
        $$ = createNode(NODE_EQ);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp NE Exp
    {
        $$ = createNode(NODE_NE);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp GT Exp
    {
        $$ = createNode(NODE_GT);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp GE Exp
    {
        $$ = createNode(NODE_GE);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp LT Exp
    {
        $$ = createNode(NODE_LT);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp LE Exp
    {
        $$ = createNode(NODE_LE);
        $$->left = $1;
        $$->right = $3;
    }
    | MINUS Exp %prec UMINUS
    {
        $$ = createNode(NODE_UMINUS);
        $$->left = $2;
    }
    | LP Exp RP
    {
        $$ = createNode(NODE_PAREN_EXP);
        $$->child = $2;
    }
    ;






/************************************************************/
%%

int main(int argc, char *argv[]){
	// 如果开启源代码显示功能，先输出源代码
	#if SHOW_SOURCE_CODE
	printSourceCode(argv[1]);
	#endif
	
	yyin=fopen(argv[1],"r");
	if (!yyin) return 0;
	yylineno=1;
	yyparse();
	return 0;
	}

#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}
	

