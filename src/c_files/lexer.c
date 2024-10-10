//
// Created by Nab on 10/10/2024.
//
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes_h/lexer.h"

#define MAX_TOKEN_SIZE 100

token* createToken(const char* type, const char* value) {
    token* newToken = malloc(sizeof(token));
    newToken->type = strdup(type);
    newToken->value = strdup(value);
    newToken->nextToken = NULL;
    return newToken;
}

token* lexerCalculator(char* input) {
    token *firstToken = NULL;
    token *currentToken = NULL;
    char buffer[MAX_TOKEN_SIZE];
    int i = 0;

    while (input[i] != '\0') {
        if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
            i++;
            continue;
        }

        // Variable
        if (isalpha(input[i])) {
            int start = i;
            while (isalpha(input[i])) {
                i++;
            }
            strncpy(buffer, &input[start], i - start);
            buffer[i - start] = '\0';  // Terminer la chaîne

            token *newToken = createToken("variable", buffer);
            if (!firstToken) {
                firstToken = newToken;
            } else {
                currentToken->nextToken = newToken;
            }
            currentToken = newToken;
        }

        // Nombre
        else if (isdigit(input[i])) {
            int start = i;
            while (isdigit(input[i])) {
                i++;
            }
            strncpy(buffer, &input[start], i - start);
            buffer[i - start] = '\0';

            token *newToken = createToken("number", buffer);
            if (!firstToken) {
                firstToken = newToken;
            } else {
                currentToken->nextToken = newToken;
            }
            currentToken = newToken;
        }

        // Opérateur
        else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
            buffer[0] = input[i];
            buffer[1] = '\0';

            token *newToken = createToken("operator", buffer);
            if (!firstToken) {
                firstToken = newToken;
            } else {
                currentToken->nextToken = newToken;
            }
            currentToken = newToken;
            i++;
        }

        // Affectation
        else if (input[i] == '=') {
            buffer[0] = input[i];
            buffer[1] = '\0';

            token *newToken = createToken("affectation", buffer);
            if (!firstToken) {
                firstToken = newToken;
            } else {
                currentToken->nextToken = newToken;
            }
            currentToken = newToken;
            i++;
        }

        // Si caractère non reconnu, on avance
        else {
            i++;
        }
    }

    return firstToken;
}