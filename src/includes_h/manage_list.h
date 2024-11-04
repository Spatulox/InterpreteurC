//
// Created by M78st on 02/10/2024.
//

#ifndef INTERPRETEURC_MANAGE_LIST_H
#define INTERPRETEURC_MANAGE_LIST_H
ListVariable* createVariableNode(TYPE type, Value value, char *name);
void addVariableToList(ListVariable** start, TYPE type, Value value, char *name);
void printListsVar(ListVariable* variableList);
void freeVariableList(ListVariable* head);
ListVariable* searchVariableInList(ListVariable* start, char* varName);

#endif //INTERPRETEURC_MANAGE_LIST_H
