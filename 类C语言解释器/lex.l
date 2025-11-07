%option yylineno
%{
#include "parser.tab.h"
#include "string.h"
#include "def.h"

int yycolumn=1;
#define YY_USER_ACTION    \
	yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;\
	yylloc.last_column=yycolumn+yyleng-1;\
	yycolumn+=yyleng;
	   	   
typedef union {
	int type_int;
	float type_float;
	char type_id[32];
} YYLVAL;
#define YYSTYPE YYLVAL
/*******************begin********************************/
%}
%option noyywrap
%x COMMENT
%%
"if"           { return IF; }
"else"         { return ELSE; }
"while"        { return WHILE; }
"print"        { return PRINT; }
"scan"         { return SCAN; }

"=="           { return EQ; }
"!="           { return NE; }
">="           { return GE; }
"<="           { return LE; }
">"            { return GT; }
"<"            { return LT; }
"="            { return ASSIGNOP; }
"+"            { return PLUS; }
"-"            { return MINUS; }
"*"            { return STAR; }
"/"            { return DIV; }
"("            { return LP; }
")"            { return RP; }
"{"            { return LC; }
"}"            { return RC; }
";"            { return SEMI; }


[0-9]+\.[0-9]+  { yylval.type_float = atof(yytext); return FLOAT; }
[0-9]+\.         { yylval.type_float = atof(yytext); return FLOAT; }
\.[0-9]+         { yylval.type_float = atof(yytext); return FLOAT; }
[0-9]+           { yylval.type_int = atoi(yytext); return INT; }

[a-zA-Z_][a-zA-Z0-9_]* {
    strncpy(yylval.type_id, yytext, sizeof(yylval.type_id)-1);
    yylval.type_id[sizeof(yylval.type_id)-1] = '\0';
    return ID;
}

[ \t\r\n]+     { /* skip whitespace */ }

"//"[^\n]*      { /* single-line comment */ }
"/*"            { BEGIN(COMMENT); }
<COMMENT>"*/"   { BEGIN(INITIAL); }
<COMMENT>.|\n   { /* eat comment content */ }

.               { /* unknown char, skip */ }
%%
/*******************end********************************/