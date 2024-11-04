//
// Created by M78st on 02/10/2024.
//
#include <stdio.h>
#include <stdlib.h>

#include "../includes_h/structs.h"
#include "../includes_h/log.h"

// ------------------------------------------------------------------------ //

ListVariable* createVariableNode(TYPE type, Value value) {
    ListVariable* node = malloc(sizeof(ListVariable));
    if (node) {
        node->TYPE = type;
        node->value = value;
        node->next = NULL;
    } else {
        Log("ERROR : Impossible to create a ListVariable node");
    }
    return node;
}

// ------------------------------------------------------------------------ //

void addVariableToList(ListVariable** start, TYPE type, Value value) {
    ListVariable* newNode = createVariableNode(type, value);
    if (newNode) {
        newNode->next = (struct ListVariable *) *start;
        *start = newNode;
    } else {
        Log("ERROR : Impossible to add a node to the ListVariable");
    }
}

// ------------------------------------------------------------------------ //

void freeVariableList(ListVariable* head) {
    while (head) {
        ListVariable* temp = head;
        head = (ListVariable *) head->next;
        free(temp);
    }
}

// ------------------------------------------------------------------------ //

void printListsVar(ListVariable* variableList) {
    printf("Details of ListVariables:\n");
    ListVariable *currentVar = variableList;
    while (currentVar != NULL) {
        printf("Type: ");
        switch (currentVar->TYPE) {
            case INT:
                printf("INT, Value: %d\n", currentVar->value.intValue);
                break;
            case CHAR:
                printf("CHAR, Value: %s\n", currentVar->value.stringValue);
                break;
            default:
                printf("Unknown type\n");
        }
        currentVar = currentVar->next;
    }
}