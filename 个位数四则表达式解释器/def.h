#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    NODE_DIGIT,
    NODE_PLUS,
    NODE_MINUS,
    NODE_TIMES,
    NODE_DIVIDE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int value;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode* create_digit_node(int value);
ASTNode* create_op_node(NodeType type, ASTNode *left, ASTNode *right);
void print_tree(ASTNode *node, int depth);
int evaluate(ASTNode *node);
void free_tree(ASTNode *node);

#endif

