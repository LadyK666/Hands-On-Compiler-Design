#include "def.h"

ASTNode* create_digit_node(int value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_DIGIT;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* create_op_node(NodeType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = 0;
    node->left = left;
    node->right = right;
    return node;
}

void print_tree(ASTNode *node, int depth) {
    if (node == NULL) return;
    
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    switch (node->type) {
        case NODE_DIGIT:
            printf("DIGIT: %d\n", node->value);
            break;
        case NODE_PLUS:
            printf("PLUS\n");
            break;
        case NODE_MINUS:
            printf("MINUS\n");
            break;
        case NODE_TIMES:
            printf("TIMES\n");
            break;
        case NODE_DIVIDE:
            printf("DIVIDE\n");
            break;
    }
    
    if (node->left != NULL) {
        print_tree(node->left, depth + 1);
    }
    if (node->right != NULL) {
        print_tree(node->right, depth + 1);
    }
}

int evaluate(ASTNode *node) {
    if (node == NULL) return 0;
    
    switch (node->type) {
        case NODE_DIGIT:
            return node->value;
        case NODE_PLUS:
            return evaluate(node->left) + evaluate(node->right);
        case NODE_MINUS:
            return evaluate(node->left) - evaluate(node->right);
        case NODE_TIMES:
            return evaluate(node->left) * evaluate(node->right);
        case NODE_DIVIDE:
            {
                int right_val = evaluate(node->right);
                if (right_val == 0) {
                    printf("Error: Division by zero!\n");
                    return 0;
                }
                return evaluate(node->left) / right_val;
            }
        default:
            return 0;
    }
}

void free_tree(ASTNode *node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

