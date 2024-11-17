//
// Created by Marc on 14/11/2024.
//

#include "stringUtils.h"

#include "includes.h"

number tryConvertToNumber(number v) {
    if (v.type != STRING) return v;

    char *endptr;
    long l = strtol(v.value.string, &endptr, 10);
    if (*endptr == '\0') {
        v.type = INT;
        v.value.int_value = (int)l;
    } else {
        double d = strtod(v.value.string, &endptr);
        if (*endptr == '\0') {
            v.type = FLOAT;
            v.value.float_value = (float)d;
        }
    }
    return v;
}

int castStringIntoNumber(number *left, number *right) {
    if (left->type == STRING) {
        *left = tryConvertToNumber(*left);
    }
    if (right->type == STRING) {
        *right = tryConvertToNumber(*right);
    }
    if (right->type == STRING || left->type == STRING) {
        printf("Impossible to cast the string into a number\n");
        return -1;
    }
    return 0;
}

void castNumberIntoString(number astNumber, char **resultString) {
    char *buffer;

    if (astNumber.type == STRING) {
        *resultString = realloc(*resultString, strlen(*resultString) + strlen(astNumber.value.string) + 1);
        if (*resultString == NULL) {
            printf("Memory alloc failed\n");
            exit(EXIT_FAILURE);
        }
        strcat(*resultString, astNumber.value.string);

    } else if (astNumber.type == INT) {
        unsigned long long length = snprintf(NULL, 0, "%d", astNumber.value.int_value);
        buffer = malloc(length + 1);
        if (buffer == NULL) {
            printf("Memory alloc failed\n");
            exit(EXIT_FAILURE);
        }
        sprintf(buffer, "%d", astNumber.value.int_value);

        *resultString = realloc(*resultString, strlen(*resultString) + length + 1);
        if (*resultString == NULL) {
            printf("Memory alloc failed\n");
            free(buffer);
            exit(EXIT_FAILURE);
        }
        strcat(*resultString, buffer);
        free(buffer);

    } else if(astNumber.type == FLOAT) {
        unsigned long long length = snprintf(NULL, 0, "%f", astNumber.value.float_value);
        buffer = malloc(length + 1);
        if (buffer == NULL) {
            printf("Memory alloc failed\n");
            exit(EXIT_FAILURE);
        }
        sprintf(buffer, "%f", astNumber.value.float_value);

        *resultString = realloc(*resultString, strlen(*resultString) + length + 1);
        if (*resultString == NULL) {
            printf("Memory alloc failed\n");
            free(buffer);
            exit(EXIT_FAILURE);
        }
        strcat(*resultString, buffer);
        free(buffer);
    }
}
