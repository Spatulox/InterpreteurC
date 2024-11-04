//
// Created by M78st on 02/10/2024.
//

#ifndef INTERPRETEURC_STRUCTS_H
#define INTERPRETEURC_STRUCTS_H
typedef enum {
    INT,
    CHAR
}TYPE;

typedef union {
    int intValue;
    char * stringValue;
} Value;

// Variable has a value and a type (to avoid wrong cast)
typedef struct {
    TYPE TYPE;
    Value value;
}Variable;

// CHAINED LIST

typedef struct {
    int TYPE;
    Value value;
    struct ListVariable *next;
}ListVariable;

#endif //INTERPRETEURC_STRUCTS_H
