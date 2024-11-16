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

    switch (var->variable.type) {
        case INT_VAR:
            var->variable.value.intValue = 0; // Initialiser à 0 pour les entiers
            break;
        case FLOAT_VAR:
            var->variable.value.floatValue = 0.0; // Initialiser à 0.0 pour les flottants
            break;
        case STRING_VAR:
            if (var->variable.value.stringValue != NULL) {
                free(var->variable.value.stringValue); // Libérer la mémoire pour les chaînes
                var->variable.value.stringValue = NULL; // Optionnel : mettre à NULL après libération
            }
            break;
        case ARRAY_VAR:
            freeOldValueVariable(var->variable.array);
            break;
        default:
            printf("ERROR: Unknown Var type\n"); // Gérer les types inconnus
            break;
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
        if(temp->variable.type == ARRAY_VAR){
            freeVariableList(temp->variable.array);
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

    // Vérifier si la liste est vide
    if (currentVar == NULL) {
        printf("La liste des variables est vide.\n");
        return;
    }

    while (currentVar != NULL) {
        printf("Name: %s, Type: ", currentVar->variable.varName);
        switch (currentVar->variable.type) {
            case INT_VAR:
                printf("INT, Value: %d, Scope: %d\n", currentVar->variable.value.intValue, currentVar->variable.scope);
                break;
            case FLOAT_VAR:
                printf("FLOAT, Value: %f, Scope: %d\n", currentVar->variable.value.floatValue, currentVar->variable.scope);
                break;
            case STRING_VAR:
                printf("STRING, Value: %s, Scope: %d\n",
                       currentVar->variable.value.stringValue ? currentVar->variable.value.stringValue : "NULL",
                       currentVar->variable.scope);
                break;
            case ARRAY_VAR:
                printf("ARRAY:\n");
                if (currentVar->variable.value.array != NULL) {
                    printListsVar(currentVar->variable.value.array); // Appel récursif pour afficher le tableau
                } else {
                    printf("Array reference is NULL\n");
                }
                break;
            default:
                printf("Unknown type\n");
        }
        currentVar = currentVar->next; // Passer à la variable suivante
    }
}