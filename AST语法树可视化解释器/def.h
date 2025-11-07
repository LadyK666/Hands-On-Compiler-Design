/***********定义结点类型和给出函数说明*********/

#ifndef DEF_H
#define DEF_H

// AST节点类型枚举
typedef enum {
    NODE_PROGRAM,      // 程序节点
    NODE_STMLIST,      // 语句列表节点
    NODE_EXP_STMT,     // 表达式语句节点
    NODE_COMP_STMT,    // 复合语句节点
    NODE_SCAN_STMT,    // 输入语句节点
    NODE_PRINT_STMT,   // 输出语句节点
    NODE_IF_STMT,      // if语句节点
    NODE_IF_ELSE_STMT, // if-else语句节点
    NODE_WHILE_STMT,   // while语句节点
    NODE_INT_CONST,    // 整型常量节点
    NODE_FLOAT_CONST,  // 浮点常量节点
    NODE_VAR,          // 变量节点
    NODE_ASSIGN,       // 赋值表达式节点
    NODE_PLUS,         // 加法表达式节点
    NODE_MINUS,        // 减法表达式节点
    NODE_STAR,         // 乘法表达式节点
    NODE_DIV,          // 除法表达式节点
    NODE_EQ,           // 等于表达式节点
    NODE_NE,           // 不等于表达式节点
    NODE_GT,           // 大于表达式节点
    NODE_GE,           // 大于等于表达式节点
    NODE_LT,           // 小于表达式节点
    NODE_LE,           // 小于等于表达式节点
    NODE_UMINUS,       // 单目减表达式节点
    NODE_PAREN_EXP     // 括号表达式节点
} NodeType;

// AST节点结构
typedef struct ASTNode {
    NodeType type;              // 节点类型
    union {
        int int_val;            // 整型常量的值
        float float_val;        // 浮点常量的值
        char var_name[32];      // 变量名
    } value;
    struct ASTNode *left;       // 左子树
    struct ASTNode *right;      // 右子树
    struct ASTNode *child;      // 子节点（用于语句列表、条件语句等）
} ASTNode;

// 功能开关：是否在显示语法树前输出源代码
// 设置为 1 开启，0 关闭
#define SHOW_SOURCE_CODE 1

// 函数声明
ASTNode* createNode(NodeType type);
ASTNode* createIntNode(int val);
ASTNode* createFloatNode(float val);
ASTNode* createVarNode(char* name);
void displayAST(ASTNode* node, int indent);
void printSourceCode(const char* filename);

#endif

/*********************************************/
