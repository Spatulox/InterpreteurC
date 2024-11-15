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

void addToken(TokenType type, char *buffer, Token **firstToken, Token **currentToken) {
    Token *newToken = createToken(type, buffer);
    if (!*firstToken) {
        *firstToken = newToken;
    } else {
        (*currentToken)->nextToken = newToken;
    }
    *currentToken = newToken;
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
        if (isalpha(input[i]) || input[i] == '_') {
            int start = i;
            i++;
            while (isalpha(input[i]) || isdigit(input[i]) || input[i] == '_') {
                i++;
            }
            strncpy(buffer, &input[start], i - start);
            buffer[i - start] = '\0';  // Terminer la chaîne

            // Si print
            if (strcmp(buffer, "print") == 0) {
                addToken(PRINT, buffer, &firstToken, &currentToken);
            } else {
                addToken(VARIABLE, buffer, &firstToken, &currentToken);
            }
        }

        // Nombre
        else if (isdigit(input[i]) || (input[i] == '-' && isdigit(input[i + 1]) || (input[i] == '.' && isdigit(input[i+1])) && (currentToken->type != VARIABLE && currentToken->type != NUMBER))) {
            int start = i;
            if (input[i] == '-') {
                i++;
            }
            int hasDecimal = 0;
            while (isdigit(input[i]) || (input[i] == '.' && !hasDecimal)) {
                if (input[i] == '.') {
                    hasDecimal = 1;
                }
                i++;
            }
            strncpy(buffer, &input[start], i - start);
            buffer[i - start] = '\0';

            addToken(NUMBER, buffer, &firstToken, &currentToken);
        }

        // Operator // Assignment
        else if (strchr("+-*/%=", input[i])) {
            buffer[0] = input[i];
            buffer[1] = '\0';
            TokenType type;

            switch (input[i]) {
                // Opérateur
                case '+': case '-': case '*': case '/': case '%':
                    type = OPERATOR;
                    break;

                // Affectation
                case '=':
                    type = ASSIGNMENT;
                    break;
            }
            addToken(type, buffer, &firstToken, &currentToken);
            i++;
        }

        // Parenthesis / Scope
        else if (strchr("(){}", input[i])) {
            buffer[0] = input[i];
            buffer[1] = '\0';
            TokenType type;

            switch (input[i]) {
                // Parenthèses
                case '(':
                    type = PARENTHESIS_OPEN;
                    break;
                case ')':
                    type = PARENTHESIS_CLOSE;
                    break;

                // Scope
                case '{':
                    type = SCOPE_OPEN;
                    break;
                case '}':
                    type = SCOPE_CLOSE;
                    break;
            }
            addToken(type, buffer, &firstToken, &currentToken);
            i++;
        }

        

        // chaine de charactère
        else if(input[i] == '"') {
            int start = i;
            i++;
            while(input[i] != '"' || input[i] != '\0') {
                i++;
            }
            if(input[i] == '\0') {
                printf("Error: Missing closing quote\n");
                exit(1);
            }
            strncpy(buffer, &input[start], i - start);
            buffer[i - start] = '\0';
            addToken(STRING_TOKEN, buffer, &firstToken, &currentToken);
        }

        // Si caractère non reconnu, on avance
        else {
            i++;
        }
    }

    return firstToken;
}