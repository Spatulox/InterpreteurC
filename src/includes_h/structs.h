//
// Created by Marc on 02/10/2024.
//

#ifndef INTERPRETEURC_STRUCTS_H
#define INTERPRETEURC_STRUCTS_H
typedef enum {
    INT_VAR,
    FLOAT_VAR,
    STRING_VAR,
    ARRAY_VAR,
    NULL_TYPE
}Type;

typedef struct ListVariable ListVariable;

typedef union {
    int intValue;
    float floatValue;
    char * stringValue;
    ListVariable *array; // Value when referencing array (main list)
} Value;

// Variable has a value and a type (to avoid wrong cast)
typedef struct {
    Type type;
    Value value;
    char* varName;
    int scope;
    ListVariable *array; // Next value in the array
}Variable;

// CHAINED LIST

struct ListVariable {
    Variable variable;
    ListVariable *next;
};

#endif //INTERPRETEURC_STRUCTS_H
