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

ASTNode *create_comp_node(ASTNode *left, ASTNode *right, char *comp) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_COMPARE;
    node->compare.left = left;
    node->compare.right = right;
    node->compare.comp = comp;
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

//    if (*tokens
//        && ((*tokens)->type == VARIABLE || (*tokens)->type == NUMBER)
//        && (*tokens)->nextToken && (*tokens)->nextToken->type == COMPARATOR
//        && (*tokens)->nextToken->nextToken && ((*tokens)->nextToken->nextToken->type == VARIABLE || (*tokens)->nextToken->nextToken->type == NUMBER)) {
//        printf("COMPARATOR\n");
//        return parse_comp(tokens);
//    }

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

ASTNode *parse_comp(Token **tokens) {
    // 1 == 2
    ASTNode *node = parse_expression(tokens);
    *tokens = (*tokens)->nextToken;
    char *comp = strdup((*tokens)->value);
    *tokens = (*tokens)->nextToken;
    ASTNode *right = parse_expression(tokens);
    node = create_comp_node(node, right, comp);
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
    } else if (token.type == PARENTHESIS_OPEN) {
        Token *tmp = *tokens;
        while((*tokens)->type != PARENTHESIS_CLOSE){
            printf("parse_primary\n");
            *tokens = (*tokens)->nextToken;
            if ((*tokens)->type == COMPARATOR) {
                printf("if\n");
                return parse_comp(&tmp);
            }
        }
        *tokens = tmp;
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
    } else if (token.type == COMPARATOR) {

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
