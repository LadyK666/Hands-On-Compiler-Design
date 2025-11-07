%define parse.error verbose
%locations
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
/*******************begin********************************/
%union {
    int type_int;
    float type_float;
    char type_id[32];
    struct TreeNode* node;
}

%token <type_int> INT
%token <type_float> FLOAT
%token <type_id> ID
%token ASSIGNOP PLUS MINUS STAR DIV
%token EQ NE GT GE LT LE
%token LP RP LC RC SEMI
%token IF ELSE WHILE PRINT SCAN

%type <node> program StmList Stmt Exp CompSt

%right ASSIGNOP
%left EQ NE GT GE LT LE
%left PLUS MINUS
%left STAR DIV
%right UMINUS
%nonassoc IFX
%nonassoc ELSE

%%
program
    : StmList               { eval($1); }
    ;

StmList
    : /* empty */           { $$ = NULL; }
    | Stmt StmList          {
            if ($1) { $1->next = $2; $$ = $1; } else { $$ = $2; }
        }
    ;

CompSt
    : LC StmList RC         { $$ = $2; }
    ;

Stmt
    : Exp SEMI              { $$ = $1; }
    | CompSt                { $$ = $1; }
    | SCAN ID SEMI          { $$ = makeScanNode(createNode(NODE_ID, $2)); }
    | PRINT Exp SEMI        { $$ = makePrintNode($2); }
    | IF LP Exp RP Stmt %prec IFX { $$ = makeIfNode($3, $5, NULL); }
    | IF LP Exp RP Stmt ELSE Stmt { $$ = makeIfNode($3, $5, $7); }
    | WHILE LP Exp RP Stmt  { $$ = makeWhileNode($3, $5); }
    ;

Exp
    : INT                   {
            TreeNode* n = createNode(NODE_INT, "INT");
            n->int_val = $1; $$ = n;
        }
    | FLOAT                 {
            TreeNode* n = createNode(NODE_FLOAT, "FLOAT");
            n->float_val = $1; $$ = n;
        }
    | ID                    { 
            extern int yylineno;
            $$ = createNode(NODE_ID, $1);
            $$->lineno = yylineno;
        }
    | Exp ASSIGNOP Exp      { $$ = makeAssignNode($1, $3); }
    | Exp PLUS Exp          { $$ = makeOpNode("+", $1, $3); }
    | Exp MINUS Exp         { $$ = makeOpNode("-", $1, $3); }
    | Exp STAR Exp          { $$ = makeOpNode("*", $1, $3); }
    | Exp DIV Exp           { $$ = makeOpNode("/", $1, $3); }
    | Exp EQ Exp            { $$ = makeOpNode("==", $1, $3); }
    | Exp NE Exp            { $$ = makeOpNode("!=", $1, $3); }
    | Exp GT Exp            { $$ = makeOpNode(">", $1, $3); }
    | Exp GE Exp            { $$ = makeOpNode(">=", $1, $3); }
    | Exp LT Exp            { $$ = makeOpNode("<", $1, $3); }
    | Exp LE Exp            { $$ = makeOpNode("<=", $1, $3); }
    | MINUS Exp %prec UMINUS{ $$ = makeOpNode("NEG", $2, NULL); }
    | LP Exp RP             { $$ = $2; }
    ;

%%

void yyerror(const char* fmt, ...){
    fprintf(stderr, "Error at line %d near '%s'\n", yylineno, yytext);
}

/*******************end********************************/