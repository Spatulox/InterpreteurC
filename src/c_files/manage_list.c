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

// Fonctions pour gérer la liste de tokens parsés
ParsedList* createParsedNode(Token token) {
    ParsedList* node = malloc(sizeof(ParsedList));
    if (node) {
        node->token = token;
        node->next = NULL;
    } else {
        Log("ERROR : Impossible to create a Parsed node");
    }
    return node;
}

// ------------------------------------------------------------------------ //

void addParsedToList(ParsedList** head, Token token) {
    ParsedList* newNode = createParsedNode(token);
    if (newNode) {
        newNode->next = (struct ParsedList *) *head;
        *head = newNode;
    } else {
        Log("ERROR : Impossible to add a node to the ParsedList");
    }
}

// ------------------------------------------------------------------------ //

// Fonction pour libérer la mémoire
void freeVariableList(ListVariable* head) {
    while (head) {
        ListVariable* temp = head;
        head = (ListVariable *) head->next;
        free(temp);
    }
}

// ------------------------------------------------------------------------ //

void freeParsedList(ParsedList* head) {
    while (head) {
        ParsedList* temp = head;
        head = (ParsedList *) head->next;
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
            case FLOAT:
                printf("FLOAT, Value: %f\n", currentVar->value.floatValue);
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

// ------------------------------------------------------------------------ //

void printParsedLists(ParsedList* parsedList) {
    printf("Parsed List:\n");
    ParsedList* currentParsed = parsedList;
    while (currentParsed != NULL) {
        if (currentParsed->token.variable.TYPE == INT ||
            currentParsed->token.variable.TYPE == FLOAT ||
            currentParsed->token.variable.TYPE == CHAR) {
            printf("Variable - ");
            switch (currentParsed->token.variable.TYPE) {
                case INT:
                    printf("| INT, Value: %d\n", currentParsed->token.variable.value.intValue);
                    break;
                case FLOAT:
                    printf("| FLOAT, Value: %f\n", currentParsed->token.variable.value.floatValue);
                    break;
                case CHAR:
                    printf("| CHAR, Value: %s\n", currentParsed->token.variable.value.stringValue);
                    break;
            }
        } else {
            printf("Operator: ");
            switch (*currentParsed->token.operator) {
                case ADD:
                    printf("| ADD (+)\n");
                    break;

                case SUB:
                    printf("| SUB (-)\n");
                    break;

                case MUL:
                    printf("| MUL (*)\n");
                    break;

                case DIV:
                    printf("| DIV (/)\n");
                    break;

                case EQUAL:
                    printf("| EQUAL (=)\n");
                    break;

                case ASSIGN:
                    printf("| ASSIGN (:)\n");
                    break;

                case FUNC:
                    printf("| FUNC (print)\n");
                    break;

                default: printf("| Unknown operator\n");
            }
        }
        currentParsed = (ParsedList *) currentParsed->next;
    }
}