#include "stdio.h"
#include "parser.tab.h"
#include "def.h"
#include "stdlib.h"
#include "string.h"

/****************请在下面填写代码显示语法树*************/

ASTNode* createNode(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->child = NULL;
    return node;
}

ASTNode* createIntNode(int val) {
    ASTNode* node = createNode(NODE_INT_CONST);
    node->value.int_val = val;
    return node;
}

ASTNode* createFloatNode(float val) {
    ASTNode* node = createNode(NODE_FLOAT_CONST);
    node->value.float_val = val;
    return node;
}

ASTNode* createVarNode(char* name) {
    ASTNode* node = createNode(NODE_VAR);
    strcpy(node->value.var_name, name);
    return node;
}

void printIndent(int indent) {
    int i;
    for (i = 0; i < indent; i++) {
        printf("    ");
    }
}

void displayAST(ASTNode* node, int indent) {
    if (node == NULL) return;
    
    switch (node->type) {
        case NODE_PROGRAM:
            displayAST(node->child, indent);
            break;
            
        case NODE_STMLIST:
            displayAST(node->child, indent);
            if (node->right != NULL) {
                displayAST(node->right, indent);
            }
            break;
            
        case NODE_EXP_STMT:
            printIndent(indent);
            printf("表达式语句：\n");
            displayAST(node->child, indent + 1);
            break;
            
        case NODE_COMP_STMT:
            displayAST(node->child, indent);
            break;
            
        case NODE_SCAN_STMT:
            printIndent(indent);
            printf("输入变量：%s\n", node->value.var_name);
            break;
            
        case NODE_PRINT_STMT:
            printIndent(indent);
            printf("输出表达式:\n");
            displayAST(node->child, indent + 1);
            break;
            
        case NODE_IF_STMT:
            printIndent(indent);
            printf("条件语句(if_then)：\n");
            printIndent(indent + 1);
            printf("条件：\n");
            displayAST(node->child, indent + 2);
            printIndent(indent + 1);
            printf("if子句：\n");
            displayAST(node->left, indent + 2);
            break;
            
        case NODE_IF_ELSE_STMT:
            printIndent(indent);
            printf("条件语句(if_then_else)：\n");
            printIndent(indent + 1);
            printf("条件：\n");
            displayAST(node->child, indent + 2);
            printIndent(indent + 1);
            printf("if子句：\n");
            displayAST(node->left, indent + 2);
            printIndent(indent + 1);
            printf("else子句：\n");
            displayAST(node->right, indent + 2);
            break;
            
        case NODE_WHILE_STMT:
            printIndent(indent);
            printf("循环语句：\n");
            printIndent(indent + 1);
            printf("条件：\n");
            displayAST(node->child, indent + 2);
            printIndent(indent + 1);
            printf("循环体：\n");
            displayAST(node->left, indent + 2);
            break;
            
        case NODE_INT_CONST:
            printIndent(indent);
            printf("整型常量：%d\n", node->value.int_val);
            break;
            
        case NODE_FLOAT_CONST:
            printIndent(indent);
            printf("浮点常量：%f\n", node->value.float_val);
            break;
            
        case NODE_VAR:
            printIndent(indent);
            printf("变量：%s\n", node->value.var_name);
            break;
            
        case NODE_ASSIGN:
            printIndent(indent);
            printf("=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_PLUS:
            printIndent(indent);
            printf("+\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_MINUS:
            printIndent(indent);
            printf("-\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_STAR:
            printIndent(indent);
            printf("*\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_DIV:
            printIndent(indent);
            printf("/\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_EQ:
            printIndent(indent);
            printf("==\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_NE:
            printIndent(indent);
            printf("!=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_GT:
            printIndent(indent);
            printf(">\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_GE:
            printIndent(indent);
            printf(">=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_LT:
            printIndent(indent);
            printf("<\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_LE:
            printIndent(indent);
            printf("<=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_UMINUS:
            printIndent(indent);
            printf("单目-\n");
            displayAST(node->left, indent + 1);
            break;
            
        case NODE_PAREN_EXP:
            displayAST(node->child, indent);
            break;
            
        default:
            break;
    }
}

void printSourceCode(const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    
    printf("========== 源代码 ==========\n");
    char line[1024];
    int lineNum = 1;
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%3d: %s", lineNum++, line);
    }
    printf("==========================\n\n");
    fclose(fp);
}

/*****************************************************/
