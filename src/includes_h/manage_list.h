//
// Created by Marc on 02/10/2024.
//

#ifndef INTERPRETEURC_MANAGE_LIST_H
#define INTERPRETEURC_MANAGE_LIST_H

#include "structs.h"

ListVariable* createVariableNode(Type type, Value value, char *name, int scope);
void addVariableToList(ListVariable** start, Type type, Value value, char *name, int scope);
void printListsVar(ListVariable* variableList);
void freeOldValueVariable(ListVariable* var);
void freeVariableList(ListVariable* head);
void deleteVariableScopeInList(ListVariable* start, int scope);
ListVariable* searchVariableInList(ListVariable* start, char* varName);


#endif //INTERPRETEURC_MANAGE_LIST_H
