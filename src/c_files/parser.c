/*////////////////////////////////////////////////////////*/
//                                                        //   
// Name: parser.c                                     //                     
// Author: Enzo M                                         //  
// Date: 15/11/2024                                          //  
// Description: ...                                       //
//                                                        //  
/*////////////////////////////////////////////////////////*/

#include "parser.h"

ASTNode *create_number_node(const char* value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;


    char* endptr;
    double num = strtod(value, &endptr);

    if (*endptr == '\0') {
        // Ok
        if (num == floor(num)) {
            node->number.type = INT;
            node->number.value.int_value = (int) num;
        } else {
            node->number.type = FLOAT;
            node->number.value.float_value = (float) num;
        }
    } else {
        printf("IMPOSSIBLE TO DETERMINE THE TYPE OF THE NUMBER (INT OF FLOAT)");
        return 0;
    }
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

ASTNode *create_string_op_node(ASTNode *left, ASTNode *right, char op) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_STRING_OP;
    node->binary_op.op = op;
    node->binary_op.left = left;
    node->binary_op.right = right;
    return node;
}

ASTNode *create_string_node(Token** tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_STRING;
    node->number.type = STRING;
    node->number.value.string = strdup((*tokens)->value);
    *tokens = (*tokens)->nextToken;
    return node;
}

// [{NUMBER, 1, SUITE}, {OPERATOR, +, SUITE}, {NUMBER, 2, SUITE}]
ASTNode *parse_expression(Token **tokens) {
    if(tokens == NULL){
        return NULL;
    }

    if (*tokens && (*tokens)->type == VARIABLE && (*tokens)->nextToken && (*tokens)->nextToken->type == ASSIGNMENT) {
        return parse_assignment(tokens);
    }

    ASTNode *node;

    // When starting with string
    if (*tokens && (*tokens)->type == STRING_TOKEN) {
        node = create_string_node(tokens);
    } else {
        node = parse_term(tokens);
    }

    while (*tokens != NULL && (*tokens)->type == OPERATOR /*&&
           ((*tokens)->value[0] == '+' || (*tokens)->value[0] == '-')*/) {
        char op = (*tokens)->value[0];
        *tokens = (*tokens)->nextToken;

        // After an operator
        ASTNode *right;
        if ((*tokens)->type == STRING_TOKEN) {
            right = create_string_node(tokens);
        } else {
            right = parse_term(tokens);
        }

        if (node->type == AST_STRING || right->type == AST_STRING) {
            node = create_string_op_node(node, right, op);
        } else {
            node = create_binary_op_node(node, right, op);
        }
    }

    return node;
}

ASTNode *parse_term(Token **tokens) {
    ASTNode *node = parse_primary(tokens);

    while (*tokens != NULL && (*tokens)->type == OPERATOR &&
           ((*tokens)->value[0] == '*' || (*tokens)->value[0] == '/' || (*tokens)->value[0] == '%')) {
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
        ASTNode *node = create_number_node(token.value);
        *tokens = (*tokens)->nextToken;
        return node;
    } else if (token.type == VARIABLE) {
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = AST_VARIABLE;
        node->variable.name = strdup(token.value);
        *tokens = (*tokens)->nextToken;
        return node;
    }
    else if (token.type == STRING_TOKEN) {
        Token *tokenTmp = *tokens;
        ASTNode *node = create_string_node(&tokenTmp);
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
    } else if (token.type == SCOPE_OPEN) {
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = AST_SCOPE_OPEN;
        *tokens = (*tokens)->nextToken;
        return node;
    } else if (token.type == SCOPE_CLOSE) {
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = AST_SCOPE_CLOSE;
        *tokens = (*tokens)->nextToken;
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