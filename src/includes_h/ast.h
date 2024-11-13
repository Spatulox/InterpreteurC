/*////////////////////////////////////////////////////////*/
//                                                        //   
// Name: ast.h                                     //                     
// Author: Enzo M                                         //  
// Date: 12/11/2024                                          //  
// Description: ...                                       //
//                                                        //  
/*////////////////////////////////////////////////////////*/

#ifndef INTERPRETEURC_AST_H
#define INTERPRETEURC_AST_H

#include "lexer.h"
#include <stdint.h>

typedef enum {
    AST_NUMBER,
    AST_VARIABLE,
    AST_ASSIGNMENT,
    AST_BINARY_OP,
    AST_PRINT
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        struct { float value; } number;
        struct { char *name; } variable;
        struct {
            char op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;
        struct {
            char *name;
            struct ASTNode *value;
        } assignment;
        struct {
            struct ASTNode *value;
        } print;
    };
} ASTNode;

typedef struct Node {
    char *value;
    TokenType type;
    struct Node *left;
    struct Node *right;
} Node;

ASTNode *create_number_node(int value);
ASTNode *create_binary_op_node(ASTNode *left, ASTNode *right, char op);
ASTNode* parse_expression(Token **tokens);
ASTNode* parse_term(Token **tokens);
ASTNode* parse_primary(Token **tokens);
ASTNode* parse_assignment(Token **tokens);
float eval(ASTNode *node);
void free_ast(ASTNode *node);

#endif //INTERPRETEURC_AST_H
