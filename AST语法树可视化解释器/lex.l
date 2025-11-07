%option yylineno
%{
#include "parser.tab.h"
#include "string.h"
#include "stdlib.h"
#include "def.h"

int yycolumn=1;
#define YY_USER_ACTION    \
	yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;\
	yylloc.last_column=yycolumn+yyleng-1;\
	yycolumn+=yyleng;

%}
id   [A-Za-z][A-Za-z0-9]*  
int    [0-9]+
float ([0-9]*\.[0-9]+)|([0-9]+\.)
%%
"{" { return LC; }
"}" { return RC; }
"(" { return LP; }
")" { return RP; }
";" { return SEMI; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"scan" { return SCAN; }
"print" { return PRINT; }
"=" { return ASSIGNOP; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return STAR; }
"/" { return DIV; }
"==" { return EQ; }
"!=" { return NE; }
">=" { return GE; }
"<=" { return LE; }
">" { return GT; }
"<" { return LT; }
{int} { 
    yylval.type_int = atoi(yytext); 
    return INT; 
}
{float} { 
    yylval.type_float = atof(yytext); 
    return FLOAT; 
}
{id} { 
    strcpy(yylval.type_id, yytext); 
    return ID; 
}
[ \t] { /* 忽略空白字符 */ }
\n { yycolumn = 1; }
. { /* 忽略其他字符 */ }
%%

int yywrap()
{
return 1;
}

