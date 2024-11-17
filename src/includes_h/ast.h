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
#include "../includes_h/structs.h"

typedef enum {
    AST_NUMBER,
    AST_STRING,
    AST_VARIABLE,
    AST_ASSIGNMENT,
    AST_BINARY_OP,
    AST_STRING_OP,
    AST_PRINT,
    AST_SCOPE_OPEN,
    AST_SCOPE_CLOSE,
    AST_ARRAY_DECLARATION,
    AST_ARRAY_ACCESS,
    AST_ARRAY_ASSIGNMENT
} ASTNodeType;

typedef struct{
    enum{
        INT,
        FLOAT,
        STRING,
        ARRAY
    } type;
    union {
        int int_value;
        float float_value;
        char* string;
        ListVariable *array;
    } value;
} number;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        number number;
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
            char *name;
            struct ASTNode **elements;
            int size;
        } array_declaration;
        struct {
            char *name;
            struct ASTNode *index;
        } array_access;
        struct {
            char *name;
            struct ASTNode *index;
            struct ASTNode *value;
        } array_assignment;
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

//ASTNode *create_number_node(int value);
number eval(ASTNode *node);
void free_ast(ASTNode *node);

#endif //INTERPRETEURC_AST_H
