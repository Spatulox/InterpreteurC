//
// Created by M78st on 02/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes_h/structs.h"
#include "../includes_h/log.h"

// ------------------------------------------------------------------------ //


ListVariable* createVariableNode(TYPE type, Value value, char* varName) {
    ListVariable* node = malloc(sizeof(ListVariable));
    if (node) {
        node->TYPE = type;
        node->value = value;
        node->varName = strdup(varName);  // Copie le nom de la variable
        node->next = NULL;
    } else {
        Log("ERROR : Impossible to create a ListVariable node");
    }
    return node;
}

// ------------------------------------------------------------------------ //

void addVariableToList(ListVariable** start, TYPE type, Value value, char* varName) {
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
        if (strcmp(current->varName, varName) == 0) {
            return current;
        }
        current = (ListVariable*)current->next;
    }

    return NULL;
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
        printf("Name: %s, Type: ", currentVar->varName);
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