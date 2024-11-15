//
// Created by Marc on 02/10/2024.
//

#ifndef INTERPRETEURC_MANAGE_LIST_H
#define INTERPRETEURC_MANAGE_LIST_H

#include "structs.h"

ListVariable* createVariableNode(Type type, Value value, char *name);
void addVariableToList(ListVariable** start, Type type, Value value, char *name);
void printListsVar(ListVariable* variableList);
void freeOldValueVariable(ListVariable* var);
void freeVariableList(ListVariable* head);
ListVariable* searchVariableInList(ListVariable* start, char* varName);

#endif //INTERPRETEURC_MANAGE_LIST_H
