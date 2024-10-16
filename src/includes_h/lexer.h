//
// Created by Nab on 10/10/2024.
//

#ifndef LEXER_H
#define LEXER_H
#define MAX_TOKEN_SIZE 100
#include "shunting-yard.h"
typedef struct Token {
    TokenType type;
    char *value;
    struct Token *nextToken;
} Token;
Token* createToken(const char* type, const char* value);
Token* freeAllTokens(Token* firstToken);
Token* lexerCalculator(char* input);
#endif //LEXER_H
