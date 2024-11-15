/*////////////////////////////////////////////////////////*/
//                                                        //   
// Name: ast.c                                            //                     
// Author: Enzo M                                         //  
// Date: 12/11/2024                                       //  
// Description: ...                                       //
//                                                        //  
/*////////////////////////////////////////////////////////*/

#include <math.h>

#include "ast.h"
#include "includes.h"
#include "lexer.h"
#include "structs.h"
#include "manage_list.h"
#include "global.h"
#include "stringUtils.h"

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

number eval(ASTNode *node) {
    if (node == NULL) {
        printf("Error: NULL node\n");
        exit(1);
    }
    number result;
    switch (node->type) {
        case AST_NUMBER:
//            printf("Evaluating number: %d\n", node->number.value.int_value);
            if (node->number.type == INT) {
                result.type = INT;
                result.value.int_value = node->number.value.int_value;
            } else {
                result.type = FLOAT;
                result.value.float_value = node->number.value.float_value;
            }
            return result;
        case AST_VARIABLE: {
            if (node->variable.name == NULL) {
                printf("Error: Variable name is NULL\n");
                exit(1);
            }
            ListVariable *var = searchVariableInList(globalVariableList, node->variable.name);
            if (var) {
//                printf("Evaluating variable: %s with value %d\n", node->variable.name, var->variable.value.intValue);
                if(var->variable.type == INT_VAR){
                    result.type = INT;
                    result.value.int_value = var->variable.value.intValue;
                    return result;
                } else if (var->variable.type == FLOAT_VAR){
                    result.type = FLOAT;
                    result.value.float_value = var->variable.value.floatValue;
                    return result;
                } else if(var->variable.type == STRING_VAR){
                    result.type = STRING;
                    result.value.string = var->variable.value.stringValue;
                    return result;
                }
            } else {
                printf("Error: Undefined variable %s\n", node->variable.name);
                exit(1);
            }
        }
        case AST_BINARY_OP: {
            number left = eval(node->binary_op.left);
            number right = eval(node->binary_op.right);
//            printf("Evaluating binary operation: %c with left=%d and right=%d\n", node->binary_op.op, left, right);
            switch (node->binary_op.op) {
                case '+': {
                    if (left.type == STRING || right.type == STRING ) {
                        unsigned long long len = 0;
                        unsigned long long lenRight = 0;
                        unsigned long long lenLeft = 0;

                        char *resultString = malloc(1);
                        if (resultString == NULL) {
                            printf("Erreur d'allocation mémoire\n");
                            number nullValue;
                            nullValue.type = NULL_TYPE;
                            return nullValue;
                        }
                        resultString[0] = '\0';
                        castNumberIntoString(left, &resultString);
                        castNumberIntoString(right, &resultString);
                        result.type = STRING;
                        result.value.string = resultString;
                        return result;
                    }

                    if (left.type == INT && right.type == INT) {
                        result.type = INT;
                        result.value.int_value = left.value.int_value + right.value.int_value;
                        return result;
                    }
                    result.type = FLOAT;
                    result.value.float_value =
                            (left.type == INT ? (float) left.value.int_value : left.value.float_value) +
                            (right.type == INT ? (float) right.value.int_value : right.value.float_value);
                    return result;
                }

                case '-':
                    if(castStringIntoNumber(&left, &right) != 0){
                        result.type = NULL_TYPE;
                        return result;
                    }

                    if (left.type == INT && right.type == INT) {
                        result.type = INT;
                        result.value.int_value = left.value.int_value - right.value.int_value;
                        return result;
                    }
                    result.type = FLOAT;
                    result.value.float_value =
                            (left.type == INT ? (float)left.value.int_value : left.value.float_value) -
                            (right.type == INT ? (float)right.value.int_value : right.value.float_value);
                    return result;

                case '*':
                    if(castStringIntoNumber(&left, &right) != 0){
                        result.type = NULL_TYPE;
                        return result;
                    }

                    if (left.type == INT && right.type == INT) {
                        result.type = INT;
                        result.value.int_value = left.value.int_value * right.value.int_value;
                    } else {
                        result.type = FLOAT;
                        result.value.float_value =
                                (left.type == INT ? (float)left.value.int_value : left.value.float_value) *
                                (right.type == INT ? (float)right.value.int_value : right.value.float_value);
                    }
                    return result;

                case '/':
                    if(castStringIntoNumber(&left, &right) != 0){
                        result.type = NULL_TYPE;
                        return result;
                    }

                    if ((right.type == INT && right.value.int_value == 0) ||
                        (right.type == FLOAT && right.value.float_value == 0.0f)) {
                        printf("Error: Division by zero\n");
                        exit(1);
                    }
                    result.type = FLOAT;
                    result.value.float_value =
                            (left.type == INT ? (float)left.value.int_value : left.value.float_value) /
                            (right.type == INT ? (float)right.value.int_value : right.value.float_value);
                    return result;

                case '%':
                    if(castStringIntoNumber(&left, &right) != 0){
                        result.type = NULL_TYPE;
                        return result;
                    }

                    if ((right.type == INT && right.value.int_value == 0) ||
                        (right.type == FLOAT && right.value.float_value == 0.0f)) {
                        printf("Error: Modulo by zero\n");
                        exit(1);
                    }
                    if (left.type == INT && right.type == INT) {
                        result.type = INT;
                        result.value.int_value = left.value.int_value % right.value.int_value;
                    } else {
                        result.type = FLOAT;
                        result.value.float_value = fmodf(
                                (left.type == INT ? (float)left.value.int_value : left.value.float_value),
                                (right.type == INT ? (float)right.value.int_value : right.value.float_value));
                    }
                    return result;

                default:
                    printf("Unknown operator %c\n", node->binary_op.op);
                    exit(1);
            }
        }
        case AST_ASSIGNMENT: {
            number value = eval(node->assignment.value);
            ListVariable *existingVar = searchVariableInList(globalVariableList, node->assignment.name);
            if (existingVar) {
                freeOldValueVariable(existingVar);
                existingVar->variable.type = value.type;
                if (value.type == INT) {
                    existingVar->variable.value.intValue = value.value.int_value;
                } else if(value.type == FLOAT) {
                    existingVar->variable.value.floatValue = value.value.float_value;
                } else if(value.type == STRING) {
                    existingVar->variable.value.stringValue = strdup(value.value.string);
                }
            } else {
                if (value.type == INT) {
                    addVariableToList(&globalVariableList, INT_VAR, (Value) {.intValue = value.value.int_value}, node->assignment.name);
                } else if(value.type == FLOAT) {
                    addVariableToList(&globalVariableList, FLOAT_VAR, (Value) {.floatValue = value.value.float_value}, node->assignment.name);
                } else if(value.type == STRING) {
                    addVariableToList(&globalVariableList, STRING_VAR, (Value) {.stringValue = strdup(value.value.string)}, node->assignment.name);
                }
            }
            // if (value.type == INT) {
            //     printf("Assigned variable: %s = %d\n", node->assignment.name, value.value.int_value);
            // } else if (value.type == FLOAT) {
            //     printf("Assigned variable: %s = %f\n", node->assignment.name, value.value.float_value);
            // }
            return value;
        }
        case AST_PRINT: {
            number value = eval(node->print.value);
            if (value.type == INT) {
                printf("PRINT -> %d\n", value.value.int_value);
            } else if(value.type == FLOAT) {
                printf("PRINT -> %f\n", value.value.float_value);
            } else if(value.type == STRING) {
                printf("PRINT -> %s\n", value.value.string);
            } else {
                printf("Wrong var type");
            }
            return value;
        }

        default:
            printf("Unknown node type %d\n", node->type);
            exit(1);
    }
}