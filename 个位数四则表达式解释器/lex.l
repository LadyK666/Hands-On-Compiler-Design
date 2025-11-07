%{
/* =============================================================
 * lex.l  —— 词法分析程序（Lex/Flex）
 * 教学案例：个位数四则运算表达式
 * 文法：
 *   E → E + T | E - T | T
 *   T → T * F | T / F | F
 *   F → (E) | digit
 *
 * 作用：
 *   1. 将输入的表达式字符串分解为 Token（词法单元）
 *   2. 每个 Token 对应一个最小可识别的语法元素（+、-、digit等）
 * =============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

%}

/* =============================
 *  词法规则部分
 * ============================= */

%%
[0-9]          { 
                  printf("Token: DIGIT, value = %s\n", yytext);
                  return DIGIT;
                }

"+"             { 
                  printf("Token: PLUS\n");
                  return PLUS;
                }

"-"             { 
                  printf("Token: MINUS\n");
                  return MINUS;
                }

"*"             { 
                  printf("Token: TIMES\n");
                  return TIMES;
                }

"/"             { 
                  printf("Token: DIVIDE\n");
                  return DIVIDE;
                }

"("             { 
                  printf("Token: LPAREN\n");
                  return LPAREN;
                }

")"             { 
                  printf("Token: RPAREN\n");
                  return RPAREN;
                }

[ \t\r\n]+      ;

.               { 
                  printf("Error: illegal character '%s'\n", yytext);
                }

%%

/* =============================
 *  用户自定义函数部分
 * ============================= */

int yywrap(void) {
    return 1;
}
