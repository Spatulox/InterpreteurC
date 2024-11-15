//
// Created by Marc on 02/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <global.h>

#include "../includes_h/structs.h"
#include "../includes_h/log.h"

// ------------------------------------------------------------------------ //


ListVariable* createVariableNode(Type type, Value value, char* varName) {
    ListVariable* node = malloc(sizeof(ListVariable));
    if (node) {
        node->variable.type = type;
        node->variable.value = value;
        node->variable.scope = scope;
        node->variable.varName = strdup(varName);
        node->next = NULL;
    } else {
        Log("ERROR: Impossible to create a ListVariable node");
    }
    return node;
}


// ------------------------------------------------------------------------ //

void addVariableToList(ListVariable** start, Type type, Value value, char* varName) {
    ListVariable* newNode = createVariableNode(type, value, varName);
    if (newNode) {
        newNode->next = (struct ListVariable *) *start;
        *start = newNode;
    } else {
        Log("ERROR : Impossible to add a node to the ListVariable");
    }
}

// ------------------------------------------------------------------------ //

ListVariable* searchVariableInList(ListVariable* start, char* varName) {
    ListVariable* current = start;

    while (current != NULL) {
        if (strcmp(current->variable.varName, varName) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// ------------------------------------------------------------------------ //

void freeOldValueVariable(ListVariable* var) {

    if (var == NULL) {
        return;
    }

    if(var->variable.type == INT_VAR){
        var->variable.value.intValue = 0;
    } else if(var->variable.type == FLOAT_VAR){
        var->variable.value.floatValue = 0.0;
    }else if(var->variable.type == STRING_VAR){
        if(var->variable.value.stringValue != NULL){
            free(var->variable.value.stringValue);
        }
    } else {
        printf("ERROR : Unknown Var type");
    }
}

// ------------------------------------------------------------------------ //

void freeVariableList(ListVariable* head) {
    while (head) {
        ListVariable* temp = head;
        head = head->next;
        free(temp->variable.varName);
        if(temp->variable.type == STRING_VAR){
            free(temp->variable.value.stringValue);
        }
        free(temp);
    }
}

// ------------------------------------------------------------------------ //

void deleteVariableScopeInList(ListVariable** head, int scopeToDelete) {
    ListVariable* current = *head;
    ListVariable* prev = NULL;

    while (current) {
        if (current->variable.scope >= scopeToDelete) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }

            free(current->variable.varName);
            if (current->variable.type == STRING_VAR) {
                free(current->variable.value.stringValue);
            }
            ListVariable* temp = current;
            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}


// ------------------------------------------------------------------------ //

void printListsVar(ListVariable* variableList) {
    ListVariable *currentVar = variableList;
    while (currentVar != NULL) {
        printf("Name: %s, Type: ", currentVar->variable.varName);
        switch (currentVar->variable.type) {
            case INT_VAR:
                printf("INT, Value: %d\n, Scope : %d", currentVar->variable.value.intValue, currentVar->variable.scope);
                break;
            case FLOAT_VAR:
                printf("INT, Value: %f\n, Scope : %d", currentVar->variable.value.floatValue, currentVar->variable.scope);
                break;
            case STRING_VAR:
                printf("CHAR, Value: %s\n, Scope : %d", currentVar->variable.value.stringValue, currentVar->variable.scope);
                break;
            default:
                printf("Unknown type\n");
        }
        currentVar = currentVar->next;
    }
}