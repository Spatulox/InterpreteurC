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

ASTNode *create_comp_node(ASTNode *left, ASTNode *right, char *comp) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_COMPARE;
    node->compare.left = left;
    node->compare.right = right;
    node->compare.comp = comp;
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

ASTNode *parse_block(Token **tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BLOCK;
    node->block.statements = NULL;
    node->block.statement_count = 0;

    while ((*tokens)->type != SCOPE_CLOSE) {
        ASTNode *statement = parse_expression(tokens);
        node->block.statements = realloc(node->block.statements, (node->block.statement_count + 1) * sizeof(ASTNode*));
        node->block.statements[node->block.statement_count++] = statement;
    }

    *tokens = (*tokens)->nextToken;
    return node;
}



ASTNode *parse_condition_or_loop(Token **tokens) {
    ASTNode *node = malloc(sizeof(ASTNode));
    enum TokenType typeBkp = (*tokens)->type;

    if ((*tokens)->type != IF && (*tokens)->type != WHILE) {
        printf("Error: Expected 'if' or 'while' keyword\n");
        exit(1);
    }
    *tokens = (*tokens)->nextToken;

    // Parser la condition
    ASTNode *condition = parse_expression(tokens);

    // Vérifier le début du bloc
    if ((*tokens)->type != SCOPE_OPEN) {
        printf("Error: Expected '{' after %s condition\n", typeBkp == IF ? "if" : "while");
        exit(1);
    }
    *tokens = (*tokens)->nextToken;

    if (typeBkp == IF) {
        node->type = AST_IF;
        node->if_statement.condition = condition;
        node->if_statement.if_branch = parse_block(tokens);
        node->if_statement.else_branch = NULL;

        // Gérer le "else" pour les conditions "if"
        if ((*tokens)->type == ELSE) {
            *tokens = (*tokens)->nextToken;

            if ((*tokens)->type == SCOPE_OPEN) {
                *tokens = (*tokens)->nextToken;
                node->if_statement.else_branch = parse_block(tokens);
            } else if ((*tokens)->type == IF) {
                // else if
                node->if_statement.else_branch = parse_condition_or_loop(tokens);
            } else {
                printf("Error: Expected '{' or 'if' after 'else'\n");
                exit(1);
            }
        }
    } else {
        node->type = AST_LOOP;
        node->while_loop.condition = condition;
        node->while_loop.body = parse_block(tokens);
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

ASTNode *parse_comparator(Token **tokens) {
    ASTNode *left = parse_expression(tokens);

    if ((*tokens)->type != COMPARATOR) {
        printf("Error: Expected comparator\n");
        exit(1);
    }

    char *comp = strdup((*tokens)->value);
    *tokens = (*tokens)->nextToken;

    ASTNode *right = parse_expression(tokens);

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_COMPARE;
    node->compare.left = left;
    node->compare.right = right;
    node->compare.comp = comp;

    return node;
}

ASTNode *parse_primary(Token **tokens) {
    Token token = **tokens;
    ASTNode *node;

    switch (token.type) {
        case NUMBER:
            node = create_number_node(token.value);
            *tokens = (*tokens)->nextToken;
            return node;

        case VARIABLE:
            node = malloc(sizeof(ASTNode));
            node->type = AST_VARIABLE;
            node->variable.name = strdup(token.value);
            *tokens = (*tokens)->nextToken;
            return node;

        case STRING_TOKEN:
        {
            Token *tokenTmp = *tokens;
            node = create_string_node(&tokenTmp);
            *tokens = (*tokens)->nextToken;
            return node;
        }

        /*case PARENTHESIS_OPEN:
        {
            Token *tmp = *tokens;
            while((*tokens)->type != PARENTHESIS_CLOSE){
                printf("parse_primary\n");
                *tokens = (*tokens)->nextToken;
                if ((*tokens)->type == COMPARATOR) {
                    printf("comparator\n");
                    return parse_comparator(&tmp);
                }
            }
            *tokens = tmp;
            *tokens = (*tokens)->nextToken;
            node = parse_expression(tokens);
            if ((*tokens)->type == PARENTHESIS_CLOSE) {
                *tokens = (*tokens)->nextToken;
            } else {
                printf("Error: Missing closing parenthesis\n");
                exit(1);
            }
            return node;
        }*/

        case PARENTHESIS_OPEN:
        {
            *tokens = (*tokens)->nextToken;
            node = parse_expression(tokens);

            if ((*tokens)->type == COMPARATOR) {
                char *comp = strdup((*tokens)->value);
                *tokens = (*tokens)->nextToken;
                ASTNode *right = parse_expression(tokens);
                node = create_comp_node(node, right, comp);
                free(comp);
            }

            if ((*tokens)->type == PARENTHESIS_CLOSE) {
                *tokens = (*tokens)->nextToken;
            } else {
                printf("Error: Missing closing parenthesis\n");
                exit(1);
            }

            return node;
        }

        case SCOPE_OPEN:
            node = malloc(sizeof(ASTNode));
            node->type = AST_SCOPE_OPEN;
            *tokens = (*tokens)->nextToken;
            return node;

        case SCOPE_CLOSE:
            node = malloc(sizeof(ASTNode));
            node->type = AST_SCOPE_CLOSE;
            *tokens = (*tokens)->nextToken;
            return node;

        case PRINT:
            *tokens = (*tokens)->nextToken;
            node = malloc(sizeof(ASTNode));
            node->type = AST_PRINT;
            node->print.value = parse_expression(tokens);
            return node;

        case COMPARATOR:
            return parse_comparator(tokens);
            break;

        case IF:
            *tokens = (*tokens)->nextToken;
            node = malloc(sizeof(ASTNode));
            node->type = AST_IF;
            node->print.value = parse_expression(tokens);
            return node;
        case ELSE:
        case WHILE:
            *tokens = (*tokens)->nextToken;
            node = malloc(sizeof(ASTNode));
            node->type = AST_LOOP;
            node->print.value = parse_expression(tokens);
            return node;

        default:
            printf("Error: Unrecognized token in parse_primary\n");
            exit(1);
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