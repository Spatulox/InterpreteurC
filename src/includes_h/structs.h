//
// Created by M78st on 02/10/2024.
//

#ifndef INTERPRETEURC_STRUCTS_H
#define INTERPRETEURC_STRUCTS_H
typedef enum {
    INT,
    FLOAT,
    CHAR
}TYPE;

typedef enum {
    ADD, // Addition "+"
    SUB, // soustraciton "-"
    MUL, // Multiplication "*"
    DIV, // Division "/"
    EQUAL, // equal "="
    ASSIGN, // Assignation ":"
    FUNC // Function print("")
}OPERATOR;

typedef union {
    int intValue;
    float floatValue;
    char * stringValue;
} Value;

// Variable has a value and a type (to avoid wrong cast)
typedef struct {
    TYPE TYPE;
    Value value;
}Variable;

typedef union{
    Variable variable;
    OPERATOR * operator;
} Token;



// CHAINED LIST

typedef struct {
    int TYPE;
    Value value;
    struct ListVariable *next;
}ListVariable;

typedef struct {
    Token token;
    struct ParsedList *next;
}ParsedList;

#endif //INTERPRETEURC_STRUCTS_H
