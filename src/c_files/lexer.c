//
// Created by Nab on 10/10/2024.
//
#include "includes.h"
#include "lexer.h"



Token* createToken(TokenType type, const char* value) {
    Token* newToken = malloc(sizeof(Token));
    newToken->type = type;
    newToken->value = strdup(value);
    newToken->nextToken = NULL;
    return newToken;
}

Token* freeAllTokens(Token* firstToken) {
    Token* currentToken = firstToken;
    while (currentToken) {
        Token* nextToken = currentToken->nextToken;
        free(currentToken->value);
        free(currentToken);
        currentToken = nextToken;
    }
    return currentToken;
}

Token* lexerCalculator(char* input) {
    Token *firstToken = NULL;
    Token *currentToken = NULL;
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

            // Si print
            if (strcmp(buffer, "print") == 0) {
                Token *newToken = createToken(PRINT, buffer);
                if (!firstToken) {
                    firstToken = newToken;
                } else {
                    currentToken->nextToken = newToken;
                }
                currentToken = newToken;
            } else {
                Token *newToken = createToken(VARIABLE , buffer);
                if (!firstToken) {
                    firstToken = newToken;
                } else {
                    currentToken->nextToken = newToken;
                }
                currentToken = newToken;
            }
        }

        // Nombre
        else if (isdigit(input[i])) {
            int start = i;
            while (isdigit(input[i])) {
                i++;
            }
            strncpy(buffer, &input[start], i - start);
            buffer[i - start] = '\0';

            Token *newToken = createToken(NUMBER, buffer);
            if (!firstToken) {
                firstToken = newToken;
            } else {
                currentToken->nextToken = newToken;
            }
            currentToken = newToken;
        }

        // Opérateur
        else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '%') {
            buffer[0] = input[i];
            buffer[1] = '\0';

            Token *newToken = createToken(OPERATOR, buffer);
            if (!firstToken) {
                firstToken = newToken;
            } else {
                currentToken->nextToken = newToken;
            }
            currentToken = newToken;
            i++;
        }

        // Parenthèses
        else if (input[i] == '(') {
            buffer[0] = input[i];
            buffer[1] = '\0';

            Token *newToken = createToken(PARENTHESIS_OPEN, buffer);
            if (!firstToken) {
                firstToken = newToken;
            } else {
                currentToken->nextToken = newToken;
            }
            currentToken = newToken;
            i++;
        }

        else if (input[i] == ')') {
            buffer[0] = input[i];
            buffer[1] = '\0';

            Token *newToken = createToken(PARENTHESIS_CLOSE, buffer);
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

            Token *newToken = createToken(ASSIGNMENT, buffer);
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