/*////////////////////////////////////////////////////////*/
//                                                        //   
// Name: parser.c                                         //
// Author: Enzo M                                         //  
// Date: 15/11/2024                                       //
// Description: ...                                       //
//                                                        //  
/*////////////////////////////////////////////////////////*/

#include "parser.h"
#include "manage_list.h"
#include "global.h"

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

ASTNode *parse_array_access(Token **tokens, char *array_name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ARRAY_ACCESS;
    node->array_access.name = strdup(array_name);

    *tokens = (*tokens)->nextToken; // '['
    node->array_access.index = parse_expression(tokens);

    if ((*tokens)->type != HOOK_CLOSE) {
        printf("Error: Expected ']' after array index\n");
        exit(1);
    }
    *tokens = (*tokens)->nextToken; // ']'

    return node;
}

ASTNode *parse_primary(Token **tokens) {
    if(*tokens == NULL) {
        return NULL;
    }
    Token token = **tokens;

    if (token.type == NUMBER) {
        ASTNode *node = create_number_node(token.value);
        *tokens = (*tokens)->nextToken;
        return node;
    } else if (token.type == VARIABLE) {

        *tokens = (*tokens)->nextToken;
        if ((*tokens)->type == HOOK_OPEN) {
            return parse_array_access(tokens, token.value);
        } else {
            ASTNode *node = malloc(sizeof(ASTNode));
            node->type = AST_VARIABLE;
            node->variable.name = strdup(token.value);
            return node;
        }
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

ASTNode *parse_array_declaration(Token **tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ARRAY_DECLARATION;
    node->array_declaration.name = strdup((*tokens)->value);

    if ((*tokens)->type != HOOK_OPEN) {
        printf("Error: Expected '[' after array assignment\n");
        exit(1);
    }
    *tokens = (*tokens)->nextToken; // '['

    node->array_declaration.elements = malloc(sizeof(ASTNode*) * 1);
    int capacity = 1;
    int size = 0;

    while ((*tokens)->type != HOOK_CLOSE) {
        if (size == capacity) {
            capacity *= 2;
            node->array_declaration.elements = realloc(node->array_declaration.elements, sizeof(ASTNode*) * capacity);
        }
        node->array_declaration.elements[size++] = parse_expression(tokens);

        if ((*tokens)->type == COMMA) {
            *tokens = (*tokens)->nextToken; // ','
        } else if ((*tokens)->type != HOOK_CLOSE) {
            printf("Error: Expected ',' or ']' in array declaration\n");
            exit(1);
        }
    }
    *tokens = (*tokens)->nextToken; // ']'

    node->array_declaration.size = size;
    return node;
}

ASTNode *parse_assignment(Token **tokens) {
    if ((*tokens)->type != VARIABLE) {
        printf("Error: Expected variable name at start of assignment\n");
        exit(1);
    }
    char *name = strdup((*tokens)->value);
    *tokens = (*tokens)->nextToken;

    if ((*tokens)->type != ASSIGNMENT) {
        printf("Error: Expected '=' after variable name in assignment\n");
        exit(1);
    }
    *tokens = (*tokens)->nextToken; // '='

    if ((*tokens)->type == HOOK_OPEN) {
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = AST_ASSIGNMENT;
        node->assignment.name = name;
        node->assignment.value = parse_array_declaration(tokens);
        return node;
    } else {
        ASTNode *right = parse_expression(tokens);

        if (right->type == AST_ARRAY_ACCESS) {
            ASTNode *node = malloc(sizeof(ASTNode));
            if(node == NULL){
                printf("Impossible to allocate memory inside parse assign\n");
                exit(1);
            }
            node->type = AST_ASSIGNMENT;
            node->assignment.name = name;
            node->assignment.value = right;
            return node;
        } else {
            ASTNode *node = malloc(sizeof(ASTNode));
            node->type = AST_ASSIGNMENT;
            node->assignment.name = name;
            node->assignment.value = right;
            return node;
        }
    }
}


