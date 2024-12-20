//
// Created by Nab on 10/10/2024.
// Updated by Nab / Marc
//
#include "includes.h"
#include "lexer.h"
#include "global.h"
#include "../includes_h/log.h"

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
                continue;
            // } else if (strcmp(buffer, "_ast_") == 0) {
            //     addToken(SHOW_AST, NULL, &firstToken, &currentToken);  
            } else if (strcmp(buffer, "exit") == 0) {
                freeAllTokens(firstToken);
                printf("Exiting program\n");
                exit(EXIT_SUCCESS);
            } else {
                addToken(VARIABLE, buffer, &firstToken, &currentToken);
                continue;
            }
        }

         //printLexer
         if (input[i] == '~') {
            printLexer(firstToken);
            i++;
         }
        
        // Nombre
        else if (isdigit(input[i]) ||(input[i] == '.' && isdigit(input[i+1])) || (input[i] == '-' && isdigit(input[i + 1]) && (currentToken->type != VARIABLE && currentToken->type != NUMBER))) {
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
        else if (strchr("+-*/%=#^~$", input[i])) {
            if(input[i] == '#') {
                while (input[i]!='\0') {
                    i++;
                }
                if (i >= strlen(input)) {
                    break;
                }
                continue;
            }
            buffer[0] = input[i];
            buffer[1] = '\0';
            TokenType type;

            switch (input[i]) {
                // Opérateur et commentaire
                case '+': case '-': case '^': case '*': case '/': case '%':
                    if(input[i] == '+' && input[i+1] == '+' && currentToken->type == VARIABLE) {
                        char* currentVariable = currentToken->value;
                        addToken(ASSIGNMENT, "=", &firstToken, &currentToken);
                        addToken(VARIABLE, currentVariable, &firstToken, &currentToken);
                        addToken(OPERATOR,"+", &firstToken, &currentToken);
                        addToken(NUMBER, "1", &firstToken, &currentToken);
                        i += 2;
                        continue;
                    }
                    if(input[i] == '-' && input[i+1] == '-' && currentToken->type == VARIABLE) {
                        char* currentVariable = currentToken->value;
                        addToken(ASSIGNMENT, "=", &firstToken, &currentToken);
                        addToken(VARIABLE, currentVariable, &firstToken, &currentToken);
                        addToken(OPERATOR,"-", &firstToken, &currentToken);
                        addToken(NUMBER, "1", &firstToken, &currentToken);
                        i += 2;
                        continue;
                    }
                    if(input[i] == '+' && input[i+1] == '=' && currentToken->type == VARIABLE) {
                        char* currentVariable = currentToken->value;
                        addToken(ASSIGNMENT, "=", &firstToken, &currentToken);
                        addToken(VARIABLE, currentVariable, &firstToken, &currentToken);
                        addToken(OPERATOR,"+", &firstToken, &currentToken);
                        i += 2;
                        continue;
                    }
                    if(input[i] == '-' && input[i+1] == '=' && currentToken->type == VARIABLE) {
                        char* currentVariable = currentToken->value;
                        addToken(ASSIGNMENT, "=", &firstToken, &currentToken);
                        addToken(VARIABLE, currentVariable, &firstToken, &currentToken);
                        addToken(OPERATOR,"-", &firstToken, &currentToken);
                        i += 2;
                        continue;
                    }
                    if(input[i] == '*' && input[i+1] == '=' && currentToken->type == VARIABLE) {
                        char* currentVariable = currentToken->value;
                        addToken(ASSIGNMENT, "=", &firstToken, &currentToken);
                        addToken(VARIABLE, currentVariable, &firstToken, &currentToken);
                        addToken(OPERATOR,"*", &firstToken, &currentToken);
                        i += 2;
                        continue;
                    }
                    if(input[i] == '/' && input[i+1] == '=' && currentToken->type == VARIABLE) {
                        char* currentVariable = currentToken->value;
                        addToken(ASSIGNMENT, "=", &firstToken, &currentToken);
                        addToken(VARIABLE, currentVariable, &firstToken, &currentToken);
                        addToken(OPERATOR,"/", &firstToken, &currentToken);
                        i += 2;
                        continue;
                    }
                    if(input[i] == '%' && input[i+1] == '=' && currentToken->type == VARIABLE) {
                        char* currentVariable = currentToken->value;
                        addToken(ASSIGNMENT, "=", &firstToken, &currentToken);
                        addToken(VARIABLE, currentVariable, &firstToken, &currentToken);
                        addToken(OPERATOR,"%", &firstToken, &currentToken);
                        i += 2;
                        continue;
                    }
                    type = OPERATOR;
                    break;

                // Affectation
                case '=':
                    type = ASSIGNMENT;
                    break;
                case '$':
                    type = SHOW_AST;
                    break;
                
                default:
                    printf("Error when detecting operator/assignment\n");
                    return NULL;
            }
            addToken(type, buffer, &firstToken, &currentToken);
            i++;
        }

        // Parenthesis / Scope
        else if (strchr("(){}[],", input[i])) {
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

                case '[':
                    type = HOOK_OPEN;
                    break;
                case ']':
                    type = HOOK_CLOSE;
                    break;
                case ',':
                    type = COMMA;
                    break;
                
                default:
                    printf("Error when detecting parenthesis/scope/hook");
                    return NULL;
            }
            addToken(type, buffer, &firstToken, &currentToken);
            i++;
        }

        

        // chaine de charactère
        else if(input[i] == '"') {
            int start = i;
            i++;
            while(input[i] != '"' && input[i] != '\0') {
                i++;
            }

            if(input[i] == '\0') {
                printf("Error: Missing closing quote\n");
                freeAllTokens(firstToken);
                exit(EXIT_FAILURE);
            }
            start++;
            strncpy(buffer, &input[start], i - start);
            buffer[i - start] = '\0';
            addToken(STRING_TOKEN, buffer, &firstToken, &currentToken);
            i++;
        }

        // Si caractère non reconnu, on avance
        else {
            i++;
        }
    }

    return firstToken;
}





void printLexer(Token *token) {
    printf("\n===== Lexer Output =====\n");
    while (token != NULL) {
        printf("Token Type: ");
        
        // Affiche le type et la valeur du token
        switch(token->type) {
            case DEFAULT:
                printf("DEFAULT       | Value: \"%s\"\n", token->value);
                break;
            case NUMBER:
                printf("NUMBER        | Value: \"%s\"\n", token->value);
                break;
            case OPERATOR:
                printf("OPERATOR      | Value: \"%s\"\n", token->value);
                break;
            case PARENTHESIS_OPEN:
                printf("PARENTHESIS_OPEN\n");
                break;
            case PARENTHESIS_CLOSE:
                printf("PARENTHESIS_CLOSE\n");
                break;
            case SCOPE_OPEN:
                printf("SCOPE_OPEN\n");
                break;
            case SCOPE_CLOSE:
                printf("SCOPE_CLOSE\n");
                break;
            case ASSIGNMENT:
                printf("ASSIGNMENT\n");
                break;
            case VARIABLE:
                printf("VARIABLE      | Value: \"%s\"\n", token->value);
                break;
            case STRING_TOKEN:
                printf("STRING_TOKEN  | Value: \"%s\"\n", token->value);
                break;
            case PRINT:
                printf("PRINT\n");
                break;
            default:
                printf("UNKNOWN\n");
        }
        
        // Flèche pour indiquer le token suivant, si présent
        if (token->nextToken != NULL) {
            printf("    |\n    V\n");
        }

        token = token->nextToken;
    }
    printf("========================\n");
}