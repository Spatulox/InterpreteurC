//
// Created by Marc on 02/10/2024.
//

#ifndef INTERPRETEURC_STRUCTS_H
#define INTERPRETEURC_STRUCTS_H
typedef enum {
    INT_VAR,
    FLOAT_VAR,
    STRING_VAR,
    NULL_TYPE
}Type;

typedef union {
    int intValue;
    float floatValue;
    char * stringValue;
} Value;

// Variable has a value and a type (to avoid wrong cast)
typedef struct {
    Type type;
    Value value;
    char* varName;
}Variable;

// CHAINED LIST

typedef struct ListVariable ListVariable;

struct ListVariable {
    Variable variable;
    ListVariable *next;
};

#endif //INTERPRETEURC_STRUCTS_H
