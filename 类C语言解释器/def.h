/************************************************/
/*                def.h 文件                    */
/************************************************/

#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***** 在此定义语法树结点类型和列出函数说明 **********/

/*---------------- 语法树节点类型 ----------------*/
typedef enum {
    NODE_PROGRAM,
    NODE_STMLIST,
    NODE_STMT,
    NODE_EXP,
    NODE_OP,
    NODE_ID,
    NODE_INT,
    NODE_FLOAT
} NodeKind;

/*---------------- 语法树节点定义 ----------------*/
typedef struct TreeNode {
    NodeKind kind;                 // 节点种类
    char name[32];                 // 名称（如ID名或操作符）
    int lineno;                    // 行号（用于错误报告）
    union {
        int int_val;               // 整型值
        float float_val;           // 浮点值
    };
    struct TreeNode *child[3];     // 子节点（最多3个）
    struct TreeNode *next;         // 用于语句链表
} TreeNode;

/*---------------- 符号表项定义 ----------------*/
typedef struct Symbol {
    char name[32];
    int type;                      // 0:int, 1:float
    union {
        int int_val;
        float float_val;
    } value;
    struct Symbol *next;
} Symbol;

/*---------------- 函数声明 ----------------*/

/* ---- 语法树相关 ---- */
TreeNode* createNode(NodeKind kind, const char *name);
TreeNode* makeOpNode(const char *op, TreeNode *left, TreeNode *right);
TreeNode* makeAssignNode(TreeNode *id, TreeNode *exp);
TreeNode* makeIfNode(TreeNode *cond, TreeNode *then_stmt, TreeNode *else_stmt);
TreeNode* makeWhileNode(TreeNode *cond, TreeNode *body);
TreeNode* makePrintNode(TreeNode *exp);
TreeNode* makeScanNode(TreeNode *id);
void printTree(TreeNode *node, int indent);

/* ---- 符号表相关 ---- */
Symbol* lookupSymbol(const char *name);
void insertSymbol(const char *name, int type);
void setSymbolValue(const char *name, int type, void *value);
void printSymbolTable();

/* ---- 语义求值 ---- */
void eval(TreeNode *node);

/************************************************/

#endif /* DEF_H */
