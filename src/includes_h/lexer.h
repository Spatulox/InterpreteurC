//
// Created by Nab on 10/10/2024.
//

#ifndef LEXER_H
#define LEXER_H
typedef struct token {
    char *type;
    char *value;
    struct token *nextToken;
} token;
token* createToken(const char* type, const char* value);
token* lexerCalculator(char* input);
#endif //LEXER_H
