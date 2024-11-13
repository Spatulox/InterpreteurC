/*////////////////////////////////////////////////////////*/
//                                                        //   
// Name: ast.c                                     //                     
// Author: Enzo M                                         //  
// Date: 12/11/2024                                          //  
// Description: ...                                       //
//                                                        //  
/*////////////////////////////////////////////////////////*/

#include "ast.h"
#include "includes.h"
#include "lexer.h"
#include "structs.h"
#include "manage_list.h"
#include "global.h"

ASTNode *create_number_node(int value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->number.value = value;
    return node;
}

ASTNode *create_binary_op_node(ASTNode *left, ASTNode *right, char op) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->binary_op.left = left;
    node->binary_op.right = right;
    node->binary_op.op = op;
    return node;
}

// [{NUMBER, 1, SUITE}, {OPERATOR, +, SUITE}, {NUMBER, 2, SUITE}]
ASTNode *parse_expression(Token **tokens) {
    if (*tokens && (*tokens)->type == VARIABLE && (*tokens)->nextToken && (*tokens)->nextToken->type == ASSIGNMENT) {
        return parse_assignment(tokens);
    }

    ASTNode *node = parse_term(tokens);

    while (*tokens != NULL && (*tokens)->type == OPERATOR &&
           ((*tokens)->value[0] == '+' || (*tokens)->value[0] == '-')) {
        char op = (*tokens)->value[0];
        *tokens = (*tokens)->nextToken;
        ASTNode *right = parse_term(tokens);
        node = create_binary_op_node(node, right, op);
    }

    return node;
}

ASTNode *parse_term(Token **tokens) {
    ASTNode *node = parse_primary(tokens);

    while (*tokens != NULL && (*tokens)->type == OPERATOR &&
           ((*tokens)->value[0] == '*' || (*tokens)->value[0] == '/')) {
        char op = (*tokens)->value[0];
        *tokens = (*tokens)->nextToken;
        ASTNode *right = parse_primary(tokens);
        node = create_binary_op_node(node, right, op);
    }

    return node;
}


ASTNode *parse_primary(Token **tokens) {
    Token token = **tokens;

    if (token.type == NUMBER) {
        ASTNode *node = create_number_node(atoi(token.value));
        *tokens = (*tokens)->nextToken;
        return node;
    } else if (token.type == VARIABLE) {
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = AST_VARIABLE;
        node->variable.name = strdup(token.value);
        *tokens = (*tokens)->nextToken;
        return node;
    } else if (token.type == PARENTHESIS_OPEN) {
        *tokens = (*tokens)->nextToken;
        ASTNode *node = parse_expression(tokens);
        if ((*tokens)->type == PARENTHESIS_CLOSE) {
            *tokens = (*tokens)->nextToken;
        } else {
            printf("Error: Missing closing parenthesis\n");
            exit(1);
        }
        return node;
    } else if (token.type == PRINT) {
        *tokens = (*tokens)->nextToken;
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = AST_PRINT;
        node->print.value = parse_expression(tokens);
        return node;
    }

    printf("Error: Unrecognized token in parse_primary\n");
    exit(1);
}


ASTNode *parse_assignment(Token **tokens) {
    if ((*tokens)->type == VARIABLE) {
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = AST_ASSIGNMENT;
        node->assignment.name = strdup((*tokens)->value);
        *tokens = (*tokens)->nextToken;

        if ((*tokens)->type == ASSIGNMENT) {
            *tokens = (*tokens)->nextToken;
            node->assignment.value = parse_expression(tokens);
            return node;
        }
    }
    printf("Error: Invalid assignment syntax\n");
    exit(1);
}



void free_ast(ASTNode *node) {
    if (node == NULL) {
        printf("Error: Ast node is null\n");
        return;
    }

    switch (node->type) {
        case AST_NUMBER:
            break;
        case AST_VARIABLE:
            free(node->variable.name);
            break;
        case AST_BINARY_OP:
            // Free child
            free_ast(node->binary_op.left);
            free_ast(node->binary_op.right);
            break;
        case AST_ASSIGNMENT:
            // Free the value
            free(node->assignment.name);
            free_ast(node->assignment.value);
            break;
        case AST_PRINT:
            // free the print
            free_ast(node->print.value);
            break;
        default:
            printf("Unknown node type %d in free_ast\n", node->type);
            break;
    }

    // free the node
    free(node);
}


float eval(ASTNode *node) {
    if (node == NULL) {
        printf("Error: NULL node\n");
        exit(1);
    }
    switch (node->type) {
        case AST_NUMBER:
//            printf("Evaluating number: %llu\n", node->number.value);
            return node->number.value;
        case AST_VARIABLE: {
            ListVariable *var = searchVariableInList(globalVariableList, node->variable.name);
            if (var) {
//                printf("Evaluating variable: %s with value %d\n", node->variable.name, var->variable.value.intValue);
                return var->variable.value.intValue;
            } else {
                printf("Error: Undefined variable %s\n", node->variable.name);
                exit(1);
            }
        }
        case AST_BINARY_OP: {
            float left = eval(node->binary_op.left);
            float right = eval(node->binary_op.right);
//            printf("Evaluating binary operation: %c with left=%d and right=%d\n", node->binary_op.op, left, right);
            switch (node->binary_op.op) {
                case '+':
                    return left + right;
                case '-':
                    return left - right;
                case '*':
                    return left * right;
                case '/':
                    if (right == 0) {
                        printf("Error: Division by zero\n");
                        exit(1);
                    }
                    return left / right;
                default:
                    printf("Unknown operator %c\n", node->binary_op.op);
                    exit(1);
            }
        }
        case AST_ASSIGNMENT: {
            float value = eval(node->assignment.value);
            ListVariable *existingVar = searchVariableInList(globalVariableList, node->assignment.name);
            if (existingVar) {
                existingVar->variable.value.intValue = value;
            } else {
                addVariableToList(&globalVariableList, INT, (Value) {.intValue = value}, node->assignment.name);
            }
//            printf("Assigned variable: %s = %d\n", node->assignment.name, value);
            return value;
        }
        case AST_PRINT:
            float value = eval(node->print.value);
            printf("PRINT -> %g\n", value);
            return value;

        default:
            printf("Unknown node type %d\n", node->type);
            exit(1);
    }
}
