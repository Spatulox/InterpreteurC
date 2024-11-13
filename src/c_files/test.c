//
// Created by M78st on 04/11/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*#include "../includes_h/test.h"*/
#include "../includes_h/structs.h"
#include "../includes_h/manage_list.h"

void test_createVariableNode();
void test_addVariableToList();
void test_searchVariableInList();

void test_all(){
    test_createVariableNode();
    test_addVariableToList();
    test_searchVariableInList();
}

void test_createVariableNode() {
    printf("Testing createVariableNode...\n");

    // Test INT
    Value intValue;
    intValue.intValue = 42;
    ListVariable* intNode = createVariableNode(INT, intValue, "intVar");

    assert(intNode != NULL);
    assert(intNode->variable.type == INT);
    assert(intNode->variable.value.intValue == 42);
    assert(strcmp(intNode->variable.varName, "intVar") == 0);
    assert(intNode->next == NULL);

    free(intNode->variable.varName);
    free(intNode);

    // Test CHAR
    Value charValue;
    charValue.stringValue = "test";
    ListVariable* charNode = createVariableNode(CHAR, charValue, "charVar");

    assert(charNode != NULL);
    assert(charNode->variable.type == CHAR);
    assert(strcmp(charNode->variable.value.stringValue, "test") == 0);
    assert(strcmp(charNode->variable.varName, "charVar") == 0);
    assert(charNode->next == NULL);

    free(charNode->variable.varName);
    free(charNode);

    printf("createVariableNode test passed!\n");
}

void test_addVariableToList() {
    printf("Testing addVariableToList...\n");

    ListVariable* list = NULL;

    // Add INT
    Value intValue;
    intValue.intValue = 10;
    addVariableToList(&list, INT, intValue, "intVar");

    assert(list != NULL);
    assert(list->variable.type == INT);
    assert(list->variable.value.intValue == 10);
    assert(strcmp(list->variable.varName, "intVar") == 0);
    assert(list->next == NULL);

    // add a CHAR
    Value charValue;
    charValue.stringValue = "hello";
    addVariableToList(&list, CHAR, charValue, "charVar");

    assert(list != NULL);
    assert(list->variable.type == CHAR);
    assert(strcmp(list->variable.value.stringValue, "hello") == 0);
    assert(strcmp(list->variable.varName, "charVar") == 0);
    assert(list->next != NULL);
    assert(((ListVariable*)list->next)->variable.type == INT);
    assert(((ListVariable*)list->next)->variable.value.intValue == 10);
    assert(strcmp(((ListVariable*)list->next)->variable.varName, "intVar") == 0);

    freeVariableList(list);

    printf("addVariableToList test passed!\n");
}


void test_searchVariableInList() {
    printf("Testing searchVariableInList...\n");

    ListVariable* list = NULL;

    // Ajouter quelques variables à la liste
    Value intValue1, intValue2, charValue;
    intValue1.intValue = 10;
    intValue2.intValue = 20;
    charValue.stringValue = strdup("hello");

    addVariableToList(&list, INT, intValue1, "intVar1");
    addVariableToList(&list, INT, intValue2, "intVar2");
    addVariableToList(&list, CHAR, charValue, "charVar");

    // Test 1: Recherche d'une variable existante (INT)
    ListVariable* result1 = searchVariableInList(list, "intVar1");
    assert(result1 != NULL);
    assert(result1->variable.type == INT);
    assert(result1->variable.value.intValue == 10);
    assert(strcmp(result1->variable.varName, "intVar1") == 0);

    // Test 2: Recherche d'une variable existante (CHAR)
    ListVariable* result2 = searchVariableInList(list, "charVar");
    assert(result2 != NULL);
    assert(result2->variable.type == CHAR);
    assert(strcmp(result2->variable.value.stringValue, "hello") == 0);
    assert(strcmp(result2->variable.varName, "charVar") == 0);

    // Test 3: Recherche d'une variable inexistante
    ListVariable* result3 = searchVariableInList(list, "nonExistentVar");
    assert(result3 == NULL);

    printListsVar(list);

    // Nettoyage
    freeVariableList(list);

    printf("searchVariableInList test passed!\n");
}