//
// Created by M78st on 02/10/2024.
//

#ifndef INTERPRETEURC_MANAGE_LIST_H
#define INTERPRETEURC_MANAGE_LIST_H
ListVariable* createVariableNode(TYPE type, Value value);
void addVariableToList(ListVariable** start, TYPE type, Value value);
void printListsVar(ListVariable* variableList);
void freeVariableList(ListVariable* head);

ParsedList* createParsedNode(Token token);
void addParsedToList(ParsedList** head, Token token);
void printParsedLists(ParsedList* parsedList);
void freeParsedList(ParsedList* head);

#endif //INTERPRETEURC_MANAGE_LIST_H
