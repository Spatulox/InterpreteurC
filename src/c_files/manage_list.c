//
// Created by Marc on 02/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes_h/structs.h"
#include "../includes_h/log.h"

// ------------------------------------------------------------------------ //


ListVariable* createVariableNode(Type type, Value value, char* varName) {
    ListVariable* node = malloc(sizeof(ListVariable));
    if (node) {
        node->variable.type = type;  // Changez TYPE par type
        node->variable.value = value;
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

void freeVariableList(ListVariable* head) {
    while (head) {
        ListVariable* temp = head;
        head = head->next;
        free(temp->variable.varName);
        free(temp);
    }
}

// ------------------------------------------------------------------------ //

void printListsVar(ListVariable* variableList) {
    ListVariable *currentVar = variableList;
    while (currentVar != NULL) {
        printf("Name: %s, Type: ", currentVar->variable.varName);
        switch (currentVar->variable.type) {
            case INTVAR:
                printf("INT, Value: %d\n", currentVar->variable.value.intValue);
                break;
            case FLOATVAR:
                printf("INT, Value: %f\n", currentVar->variable.value.floatValue);
                break;
            case CHARVAR:
                printf("CHAR, Value: %s\n", currentVar->variable.value.stringValue);
                break;
            default:
                printf("Unknown type\n");
        }
        currentVar = currentVar->next;
    }
}