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
        case AST_STRING:
            result.type = STRING;
            result.value.string = strdup(node->number.value.string);
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
                } else if(var->variable.type == ARRAY_VAR){
                    result.type = ARRAY;
                    result.value.array = var->variable.value.array;
                    return result;
                }
            } else {
                printf("Error: Undefined variable %s\n", node->variable.name);
                exit(1);
            }
        }
        case AST_STRING_OP:
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
                            exit(1);
                            //return nullValue;
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
                    if (left.type == STRING && right.type == STRING) {
                        char *endresult = strdup(left.value.string);
                        if (endresult == NULL) {
                            printf("Error: Impossible to allocate memory\n");
                            result.type = NULL_TYPE;
                            exit(1);
                            //return result;
                        }

                        int lenRight = (int)strlen(right.value.string);
                        char *currPtr, *endSubStringToEndString;

                        while ((currPtr = strstr(endresult, right.value.string)) != NULL) {
                            endSubStringToEndString = currPtr + lenRight; // chaque char dans le "reste" de la string après l'occurence
                            while (*endSubStringToEndString) {
                                *currPtr = *endSubStringToEndString;
                                *currPtr++;
                                *endSubStringToEndString++;
                            }
                            *currPtr = '\0';
                        }
                        result.type = STRING;
                        result.value.string = endresult;
                        return result;
                    }

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
                    if (left.type == STRING && right.type == FLOAT) {
                        printf("Can only split a string with an int, not a float");
                        exit(1);
                    }
                    if (left.type == STRING && right.type == INT) {
                        char *endresult = strdup(left.value.string);
                        if (endresult == NULL) {
                            printf("Error: Impossible to allocate memory\n");
                            result.type = NULL_TYPE;
                            exit(1);
                        }

                        int numParts = right.value.int_value;
                        int length = strlen(endresult);
                        int partSize = length / numParts;
                        int remainder = length % numParts;

                        ListVariable *resultList = NULL;

                        char *start = endresult;
                        for (int i = 0; i < numParts; i++) {
                            int currentPartSize = partSize + (i < remainder ? 1 : 0);
                            char *part = (char *)malloc(currentPartSize + 1);
                            if (part == NULL) {
                                printf("Error: Impossible to allocate memory for part\n");
                                exit(1);
                            }

                            strncpy(part, start, currentPartSize);
                            part[currentPartSize] = '\0';

                            addVariableToEndOfList(&resultList, STRING_VAR, (Value){.stringValue = part}, "array");

                            start += currentPartSize;
                        }

                        result.type = ARRAY_VAR;
                        result.value.array = resultList;
                        free(endresult);
                        return result;
                    }

                    if(castStringIntoNumber(&left, &right) != 0){
                        result.type = NULL_TYPE;
                        return result;
                    }

                    if ((right.type == INT && right.value.int_value == 0) ||
                        (right.type == FLOAT && right.value.float_value == 0.0f)) {
                        printf("Error: Division by zero\n");
                        exit(1);
                    }
                    float res = (left.type == INT ? (float)left.value.int_value : left.value.float_value) /
                                (right.type == INT ? (float)right.value.int_value : right.value.float_value);

                    if(res == floor(res)){
                        result.type = INT;
                        result.value.int_value = (int)res;
                        return result;
                    }
                    result.type = FLOAT;
                    result.value.float_value = res;

                    return result;

                case '%':
                    if (left.type == STRING && right.type == STRING) {
                        int lenRight = (int)strlen(right.value.string);
                        char *currPtr = left.value.string;
                        int counter = 0;

                        while ((currPtr = strstr(currPtr, right.value.string)) != NULL) {
                            counter++;
                            currPtr += lenRight;
                        }

                        result.type = INT;
                        result.value.int_value = counter;
                        return result;
                    }

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
                switch (value.type) {
                    case INT:
                        existingVar->variable.value.intValue = value.value.int_value;
                        break;
                    case FLOAT:
                        existingVar->variable.value.floatValue = value.value.float_value;
                        break;
                    case STRING:
                        if (existingVar->variable.value.stringValue != NULL) {
                            free(existingVar->variable.value.stringValue);
                        }
                        existingVar->variable.value.stringValue = strdup(value.value.string);
                        break;
                    case ARRAY_VAR: {
                        ListVariable *array = value.value.array;
                        ListVariable *nodeInter = NULL;
                        while (array != NULL) {
                            Value newValue;
                            switch (array->variable.type) {

                                case INT_VAR:
                                    newValue.intValue = array->variable.value.intValue;
                                    break;
                                case FLOAT_VAR:
                                    newValue.floatValue = array->variable.value.floatValue;
                                    break;
                                case STRING_VAR:
                                    newValue.stringValue = strdup(array->variable.value.stringValue);
                                    break;
                                case ARRAY_VAR:
                                    newValue.array = array->variable.value.array;
                                    break;
                                case NULL_TYPE:
                                    printf("Type NULL when assigning var");
                                    exit(1);
                                    break;
                            }

                            addVariableToList(&nodeInter,
                                              array->variable.type,
                                              newValue,
                                              "array");

                            array = array->next;
                        }
                        existingVar->variable.type = ARRAY_VAR;
                        existingVar->variable.value.array = nodeInter;
                        break;
                    }
                    default:
                        printf("ERROR: Unknown variable type for assignment\n");
                        break;
                }
            } else {
                switch (value.type) {
                    case INT:
                        addVariableToList(&globalVariableList, INT_VAR, (Value) {.intValue = value.value.int_value}, node->assignment.name);
                        break;
                    case FLOAT:
                        addVariableToList(&globalVariableList, FLOAT_VAR, (Value) {.floatValue = value.value.float_value}, node->assignment.name);
                        break;
                    case STRING:
                        addVariableToList(&globalVariableList, STRING_VAR, (Value) {.stringValue = strdup(value.value.string)}, node->assignment.name);
                        break;
                    case ARRAY_VAR: {
                        ListVariable *array = value.value.array;
                        ListVariable *nodeInter = NULL;
                        while (array != NULL) {
                            Value newValue;
                            switch (array->variable.type) {

                                case INT_VAR:
                                    newValue.intValue = array->variable.value.intValue;
                                    break;
                                case FLOAT_VAR:
                                    newValue.floatValue = array->variable.value.floatValue;
                                    break;
                                case STRING_VAR:
                                    newValue.stringValue = strdup(array->variable.value.stringValue);
                                    break;
                                case ARRAY_VAR:
                                    newValue.array = array->variable.value.array;
                                    break;
                                case NULL_TYPE:
                                    printf("Type NULL when assigning var");
                                    exit(1);
                                    break;
                            }

                            addVariableToList(&nodeInter,
                                              array->variable.type,
                                              newValue,
                                              "array");
                            array = array->next;
                        }
                        addVariableToList(&globalVariableList, ARRAY_VAR, (Value) {.array = nodeInter}, node->assignment.name);
                        break;
                    }
                    default:
                        printf("ERROR: Unknown variable type for new assignment\n");
                        break;
                }
            }

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
            } else if(value.type == ARRAY) {
                printf("PRINT -> [");
                ListVariable *currentArray = value.value.array;

                while(currentArray != NULL) {
                    switch (currentArray->variable.type) {
                        case INT_VAR:
                            printf("%d", currentArray->variable.value.intValue);
                            break;
                        case FLOAT_VAR:
                            printf("%f", currentArray->variable.value.floatValue);
                            break;
                        case STRING_VAR:
                            printf("\"%s\"", currentArray->variable.value.stringValue);
                            break;
                        case ARRAY_VAR:
                            printf("[ ... ]"); // Indiquer qu'il y a un tableau imbriqué
                            break;
                        case NULL_TYPE:
                            printf("NULL");
                            break;
                    }
                    if(currentArray->next != NULL){
                        printf(", ");
                    }
                    currentArray = currentArray->next; // Avancer au prochain élément de la liste
                }
                printf("]\n");
            } else {
                printf("Wrong var type\n");
            }
            return value;
        }
        case AST_SCOPE_OPEN:
            scope++;
            break;
        case AST_SCOPE_CLOSE:
            deleteVariableScopeInList(&globalVariableList, scope);
            scope--;
            if (scope < 0) {
                printf("Error: Too many closing scopes\n");
                exit(1);
            }
            break;
        default:
            printf("Unknown node type %d\n", node->type);
            exit(1);
    }
}