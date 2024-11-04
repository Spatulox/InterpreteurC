//
// Created by M78st on 04/11/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../includes_h/test.h"
#include "../includes_h/structs.h"
#include "../includes_h/manage_list.h"

void test_all(){
    test_createVariableNode();
    test_addVariableToList();
}

// Fonction de test pour createVariableNode
void test_createVariableNode() {
    printf("Testing createVariableNode...\n");

    // Test avec un INT
    Value intValue;
    intValue.intValue = 42;
    ListVariable* intNode = createVariableNode(INT, intValue);

    assert(intNode != NULL);
    assert(intNode->TYPE == INT);
    assert(intNode->value.intValue == 42);
    assert(intNode->next == NULL);

    free(intNode);

    // Test avec un CHAR
    Value charValue;
    charValue.stringValue = "test";
    ListVariable* charNode = createVariableNode(CHAR, charValue);

    assert(charNode != NULL);
    assert(charNode->TYPE == CHAR);
    assert(strcmp(charNode->value.stringValue, "test") == 0);
    assert(charNode->next == NULL);

    free(charNode);

    printf("createVariableNode test passed!\n");
}

// Fonction de test pour addVariableToList
void test_addVariableToList() {
    printf("Testing addVariableToList...\n");

    ListVariable* list = NULL;

    // Ajouter un INT
    Value intValue;
    intValue.intValue = 10;
    addVariableToList(&list, INT, intValue);

    assert(list != NULL);
    assert(list->TYPE == INT);
    assert(list->value.intValue == 10);
    assert(list->next == NULL);

    // Ajouter un CHAR
    Value charValue;
    charValue.stringValue = "hello";
    addVariableToList(&list, CHAR, charValue);

    assert(list != NULL);
    assert(list->TYPE == CHAR);
    assert(strcmp(list->value.stringValue, "hello") == 0);
    assert(list->next != NULL);
    assert(((ListVariable*)list->next)->TYPE == INT);
    assert(((ListVariable*)list->next)->value.intValue == 10);

    freeVariableList(list);

    printf("addVariableToList test passed!\n");
}